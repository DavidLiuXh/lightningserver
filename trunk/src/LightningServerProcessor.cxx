#include "LightningServerProcessor.hxx"

#include "sessionHandler.hxx"
#include "sessionmanager.hxx"
#include "sessioninfo.hxx"
#include "session.hxx"
#include "request.hxx"
#include "requestprocessor.hxx"
#include "response.hxx"
#include "responseprocessor.hxx"
#include "errorcode.hxx"

#include "userrequest.hxx"
#include "userresponse.hxx"

#include "IoUtil.hxx"

#include "util/logger.hxx"
#include "util/strutil.hxx"

#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include <boost/bind.hpp>
//----------------------------------------------------
const int MAX_WORK_THREAD_COUNT = 5;
//----------------------------------------------------
using namespace Lightning;
//----------------------------------------------------
LightningServerProcessor::LightningServerProcessor(boost::shared_ptr<event_base> eb,
            boost::shared_ptr<SessionHandler> sessionHandler,
            boost::shared_ptr<UserRequestFactory> userRequestFactory,
            boost::shared_ptr<UserResponseFactory> userResponseFactory)
:mEventBase(eb)
,mSessionHandler(sessionHandler)
,mRequestFactory(userRequestFactory)
,mResponseFactory(userResponseFactory)
,mSessionManager(new SessionManager)
,mRunning(true)
{
}
//----------------------------------------------------
void LightningServerProcessor::start()
{
    INFO(__FUNCTION__);
    startRequestProcessor();
    startResponseProcessor();
}

void LightningServerProcessor::stop()
{
    INFO(__FUNCTION__);
    mRunning = false;
    stopRequestProcessor();
    stopResponseProcessor();
}

void LightningServerProcessor::processAccepted(evutil_socket_t fd)
{
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);

    evutil_socket_t clientFD = -1;
    while((clientFD = accept(fd, (struct sockaddr *)&addr, &addrLen)) > 0)
    {
        addSession(clientFD, addr);
        memset(&addr, 0, addrLen);
    }

    int errorCode = errno;
    if (-1 == clientFD &&
                EAGAIN != errorCode &&
                EWOULDBLOCK != errorCode)
    {

        ERROR(__FUNCTION__ << "accept failed");
    }
}

void LightningServerProcessor::processTimer(evutil_socket_t fd)
{
    if (mSessionManager)
    {
        mSessionManager->freshTimeWheel();
    }
}
//----------------------------------------------------
bool LightningServerProcessor::initSession(evutil_socket_t fd,
            sockaddr_in& addr,
            boost::shared_ptr<Session>& session)
{
    bool rt = false;

    session.reset(new Session(
                    mSessionManager,
                    mRequestFactory,
                    fd,
                    inet_ntoa(addr.sin_addr)));
    if (session)
    {
        session->OnRecvRequestFinished = boost::bind(
                    &LightningServerProcessor::onRecvRequestFinished,
                    this,
                    _1,
                    _2);
        session->OnClientFdClosed = boost::bind(
                    &LightningServerProcessor::onClientFdClosed,
                    this,
                    _1);
        session->OnError= boost::bind(
                    &LightningServerProcessor::onError,
                    this,
                    _1,
                    _2);
        rt = true;
    }

    return rt;
}

void LightningServerProcessor::addSession(evutil_socket_t fd,
            sockaddr_in& addr)
{
    SessionManager::SessionPtrType session;

    if(IoUtil::setSockBlock(fd, false) &&
                mSessionManager &&
                initSession(fd, addr, session))
    {
        mSessionManager->addSession(session);

        StartRequestTask* startTask = 
            new StartRequestTask(
                        ProcessQueueItem::PRT_START,
                        session);
        if (NULL != startTask &&
                    mRequestProcessor)
        {
            mRequestProcessor->addRequest(startTask);
        }
    }
    else
    {
        ERROR(__FUNCTION__ << " | failed to set client socket non-blocking");
    }
}

void LightningServerProcessor::startRequestProcessor()
{
    DEBUG(__FUNCTION__ << " | RequestProcessor start");

    mRequestProcessor.reset(new RequestProcessor(MAX_WORK_THREAD_COUNT));
    if (mRequestProcessor)
    {
        mRequestProcessor->SessionRequestEvent = boost::bind(
                    &LightningServerProcessor::handleRequestEvent,
                    this,
                    _1,
                    _2);
        mRequestProcessor->start();
    }
}

void LightningServerProcessor::stopRequestProcessor()
{
    DEBUG(__FUNCTION__ << " | RequestProcessor stop");

    if (mRequestProcessor)
    {
        mRequestProcessor->SessionRequestEvent.clear();
        mRequestProcessor->stop();
    }

    DEBUG(__FUNCTION__ << " | RequestProcessor stop finished");
}

void LightningServerProcessor::startResponseProcessor()
{
    DEBUG(__FUNCTION__ << " | Response process start");
    mResponseProcessor.reset(new ResponseProcessor(mEventBase));
    if (mResponseProcessor)
    {
        mResponseProcessor->OnProcessResponse = boost::bind(
                    &LightningServerProcessor::handleResponseEvent,
                    this,
                    _1);
        mResponseProcessor->start();
    }
}

void LightningServerProcessor::stopResponseProcessor()
{
    DEBUG(__FUNCTION__ << " | response processor stopping");

    if (mResponseProcessor)
    {
        mResponseProcessor->OnProcessResponse.clear();
        mResponseProcessor->stop();
    }

    DEBUG(__FUNCTION__ << " | response processor stop finished");
}

void LightningServerProcessor::handleRequestEvent(RequestProcessor& requestProcessor,
            const boost::shared_ptr<ProcessQueueItem>& requestTask)
{
    if (requestTask)
    {
        switch (requestTask->getType())
        {
            case ProcessQueueItem::PRT_START:
                {
                    processStartTask(requestTask);
                }
                break;
            case ProcessQueueItem::PRT_REQUEST:
                {
                    processRequestTask(requestTask);
                }
                break;
            case ProcessQueueItem::PRT_CLISENT_CLOSED:
                {
                    processClientClosedTask(requestTask);
                }
                break;
            case ProcessQueueItem::PRT_ERROR:
                {
                    processError(requestTask);
                }
                break;
            default:
                {
                    ERROR(__FUNCTION__ << " | undefine request type");
                }
                break;
        }
    }
}

void LightningServerProcessor::handleResponseEvent(const boost::shared_ptr<ProcessQueueItem>& requestTask)
{
    DEBUG(__FUNCTION__);
 
    boost::shared_ptr<Response> response = boost::shared_dynamic_cast<Response>(
                requestTask);
    if (response)
    {
        boost::shared_ptr<Session> session = response->getSession();
        if (session)
        {
            boost::shared_ptr<UserResponse> userResponse = response->getResponse();
            if (userResponse)
            {
                size_t length = 0;
                const char* data = userResponse->serialization(length);
                session->sendData(data, length);
            }
        }
    }
}

void LightningServerProcessor::onRecvRequestFinished(boost::shared_ptr<Session> session,
            boost::shared_ptr<UserRequest> request)
{
    DEBUG(__FUNCTION__);
    ReceivedRequestTask* receivedRequestTask = 
        new ReceivedRequestTask(
                    ProcessQueueItem::PRT_REQUEST,
                    session,
                    request);
    if (NULL != receivedRequestTask &&
                mRequestProcessor)
    {
        mRequestProcessor->addRequest(receivedRequestTask);
    }
}

void LightningServerProcessor::onClientFdClosed(boost::shared_ptr<Session> session)
{
    DEBUG(__FUNCTION__);
    ClientClosedTask* clientClosedTask = 
        new ClientClosedTask(
                    ProcessQueueItem::PRT_CLISENT_CLOSED,
                    session);
    if (NULL != clientClosedTask &&
                mRequestProcessor)
    {
        mRequestProcessor->addRequest(clientClosedTask);
    }
}

void LightningServerProcessor::onError(boost::shared_ptr<Session> session, SessionErrorCode code)
{
    DEBUG(__FUNCTION__);
    ErrorTask* errorTask = 
        new ErrorTask(
                    ProcessQueueItem::PRT_ERROR,
                    session,
                    code);
    if (NULL != errorTask &&
                mRequestProcessor)
    {
        mRequestProcessor->addRequest(errorTask);
    }
}

void LightningServerProcessor::processStartTask(const boost::shared_ptr<ProcessQueueItem>& requestTask)
{
    boost::shared_ptr<StartRequestTask> startTask =
        boost::shared_dynamic_cast<StartRequestTask>(requestTask);
    boost::shared_ptr<SessionHandler> sessonHandler = mSessionHandler.lock();
    if (startTask && sessonHandler) 
    {
        boost::shared_ptr<Session> session = startTask->getSession();
        if (session)
        {
            sessonHandler->onStart(session->getSessionId(),
                        session->getInfo());

            session->init(mEventBase.lock().get());
            session->startAction(EV_READ | EV_WRITE);
        }
    }
}

void LightningServerProcessor::processClientClosedTask(const boost::shared_ptr<ProcessQueueItem>& requestTask)
{
    boost::shared_ptr<ClientClosedTask> clientClosedTask =
        boost::shared_dynamic_cast<ClientClosedTask>(requestTask);
    boost::shared_ptr<SessionHandler> sessonHandler = mSessionHandler.lock();
    if (clientClosedTask && sessonHandler) 
    {
        boost::shared_ptr<Session> session = clientClosedTask->getSession();
        if (session)
        {
            sessonHandler->onEnded(session->getSessionId());
            mSessionManager->removedSession(session);
        }
    }
}

void LightningServerProcessor::processRequestTask(const boost::shared_ptr<ProcessQueueItem>& requestTask)
{
    boost::shared_ptr<ReceivedRequestTask> task =
        boost::shared_dynamic_cast<ReceivedRequestTask>(requestTask);
    boost::shared_ptr<SessionHandler> sessonHandler = mSessionHandler.lock();
    if (task && sessonHandler)
    {
        boost::shared_ptr<UserResponseFactory> responseFactory = mResponseFactory.lock();
        if (responseFactory)
        {
            boost::shared_ptr<Session> session = task->getSession();
            boost::shared_ptr<UserResponse> response = responseFactory->create();
            
            bool isContinue = true;
            sessonHandler->onRequest((session ? session->getSessionId() : 0),
                        task->getRequest(),
                        response,
                        isContinue);
            if (!isContinue)
            {
                sessonHandler->onError(session->getSessionId(), SEC_USER_CANCEL);
                mSessionManager->removedSession(session);
                return;
            }

            Response* responseTask = new Response();
            if (NULL != responseTask)
            {
                responseTask->setResponse(response);
                responseTask->setSession(session);
                mResponseProcessor->addResponse(responseTask);          
            }
        }
    }
}

void LightningServerProcessor::processError(const boost::shared_ptr<ProcessQueueItem>& requestTask)
{
    boost::shared_ptr<ErrorTask> errorTask =
        boost::shared_dynamic_cast<ErrorTask>(requestTask);
    boost::shared_ptr<SessionHandler> sessonHandler = mSessionHandler.lock();
    if (errorTask && sessonHandler) 
    {
        boost::shared_ptr<Session> session = errorTask->getSession();
        if (session)
        {
            sessonHandler->onError(session->getSessionId(),
                        errorTask->getErrorCode());
            mSessionManager->removedSession(session);
        }
    }

}
//----------------------------------------------------

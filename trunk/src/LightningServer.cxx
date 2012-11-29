#include "LightningServer.hxx"

#include "sessionHandler.hxx"
#include "requestprocessor.hxx"

#include "IoUtil.hxx"
#include "LightningServerProcessor.hxx"
#include "userrequest.hxx"
#include "userresponse.hxx"

#include "util/logger.hxx"
#include "util/strutil.hxx"

#include <event.h>

#include <signal.h>

#include <set>

using namespace Lightning;
//------------------------------------------------------------------struct FreeEventBase
struct FreeEventBase
{
    void operator()(event_base* eb)
    {
        if (NULL != eb)
        {
            event_base_free(eb);
        }
    }
};

struct FreeEvent
{
    void operator()(event* ev)
    {
        if (NULL != ev)
        {
            event_free(ev);
        }
    }
};
//------------------------------------------------------------------class LightningServerUtil
class LightningServerUtil
{
    private:
        typedef boost::shared_ptr<event> EventPtrType;
        typedef std::set<EventPtrType> EventListType;
    public:
        static void AddExitSignalEvent(event_base* eb,
                    event_callback_fn callBackFunc,
                    void* arg,
                    int signal)
        {
            event* signalEvent = evsignal_new(eb,
                        signal,
                        callBackFunc,
                        arg);
            if (NULL != signalEvent)
            {
                EventPtrType ev(signalEvent, FreeEvent());
                evsignal_add(signalEvent, NULL);
                sEventList.insert(ev);
            }
        }

        static bool AddSockEvent(event_base* eb,
                    evutil_socket_t fd,
                    short type,
                    event_callback_fn callBackFunc,
                    void* arg,
                    const struct timeval* timeOut)
        {
            bool rt = false;

            event* sockEvent = event_new(eb,
                        fd,
                        type,
                        callBackFunc,
                        arg);
            if (NULL != sockEvent)
            {
                EventPtrType ev(sockEvent, FreeEvent());

                int code = event_add(sockEvent, timeOut);
                if (0 == code)
                {
                    sEventList.insert(ev);
                    rt = true;
                }
                else
                {
                    ERROR(__FUNCTION__ << " | invalid arguments");
                }
            }

            return rt;
        }

        static void onAccepted(evutil_socket_t fd,
                    short events,
                    void* userData)
        {
            if (events & EV_READ)
            {
                LightningServerProcessor* ls = static_cast<LightningServerProcessor*>(userData);
                if (NULL != ls)
                {
                    ls->processAccepted(fd);
                }
            }
        }

        static void onSignalHandler(evutil_socket_t, short what, void* arg)
        {
            INFO(__FUNCTION__ << " | fire signal : " << what);

            LightningServerProcessor* ls = static_cast<LightningServerProcessor*>(arg);
            if (NULL != ls)
            {
                ls->stop();
            }
        }

        static void onTimer(evutil_socket_t fd, short what, void* arg)
        {
            LightningServerProcessor* ls = static_cast<LightningServerProcessor*>(arg);
            if (ls)
            {
                ls->processTimer(fd);
            }
        }

    private:
        static EventListType sEventList;
};

LightningServerUtil::EventListType LightningServerUtil::sEventList;
//------------------------------------------------------------------class LightningServer
LightningServer::LightningServer(UserRequestFactoryPtrType userRequestFactory,
            UserResponseFactoryPtrType userResponseFactory,
            bool isDebugMode,
            const char* logPath)
:mRequestFactory(userRequestFactory)
    ,mResponseFactory(userResponseFactory)
    ,mIsDebugMode(isDebugMode)
{
    initLog(logPath);
}

LightningServer::~LightningServer()
{
}
//------------------------------------------------------------------
void LightningServer::start(const char* ip, int port)
{
    INFO(__FUNCTION__ << " | Lightning Server starting...");

    if (setupEventBase())
    {
        setupSignalHandler();

        if (startListen(ip, port))
        {
            process();
        }
    }

    INFO(__FUNCTION__ << " | Lightning Server shutdown"); 
}

void LightningServer::stop()
{
    INFO(__FUNCTION__);
}

bool LightningServer::regSessionHandler(SessionHandlerPtrType sessionHandler)
{
    mSessionHandler = sessionHandler;
    return true;
}
//------------------------------------------------------------------
void LightningServer::initLog(const char* path)
{
    if (LUtil::StrUtil::notEmptyOrNull(path))
    {
        LUtil::Logger::init(
                    mIsDebugMode ? LUtil::Logger::LL_ALL : LUtil::Logger::LL_INFO,
                    path,
                    mIsDebugMode ? true : false);
    }
}

bool LightningServer::setupEventBase()
{
    bool rt = false;

    event_base* eb = event_base_new();
    if (NULL != eb)
    {
        mEventBase.reset(eb, FreeEventBase());
        if (mEventBase)
        {
            mLSProcessor.reset(new LightningServerProcessor(
                            mEventBase,
                            mSessionHandler,
                            mRequestFactory,
                            mResponseFactory));
            if (mLSProcessor)
            {
                mLSProcessor->start();
                rt = true;
            }
        }
    }

    return rt;
}

void LightningServer::addTimerEvent()
{
    timeval tm;
    tm.tv_sec = 1;
    tm.tv_usec = 0;
    LightningServerUtil::AddSockEvent(mEventBase.get(),
                -1,
                EV_TIMEOUT | EV_PERSIST,
                LightningServerUtil::onTimer,
                mLSProcessor.get(),
                &tm);
}

void LightningServer::setupSignalHandler()
{
    signal(SIGPIPE, SIG_IGN);

    if (mEventBase)
    {
        LightningServerUtil::AddExitSignalEvent(mEventBase.get(),
                    LightningServerUtil::onSignalHandler,
                    mLSProcessor.get(),
                    SIGHUP);
        LightningServerUtil::AddExitSignalEvent(mEventBase.get(),
                    LightningServerUtil::onSignalHandler,
                    mLSProcessor.get(),
                    SIGINT);
        addTimerEvent();
    }
}

bool LightningServer::startListen(const char* ip, int port)
{
    bool rt = false;

    int listenFd = 0;
    if (IoUtil::tcpListen(ip, port, true, listenFd))
    {
        INFO(__FUNCTION__ << " | start listen"
                    << " | ip : " << LUtil::StrUtil::getSafeStr(ip)
                    << " | port : " << port);

        rt = LightningServerUtil::AddSockEvent(mEventBase.get(),
                    listenFd,
                    EV_READ | EV_PERSIST,
                    LightningServerUtil::onAccepted,
                    mLSProcessor.get(),
                    NULL);
    }

    return rt;
}

void LightningServer::process()
{
    while (mLSProcessor &&
                mLSProcessor->isRunning() &&
                mEventBase)
    {
        if (-1 == event_base_loop(mEventBase.get(), EVLOOP_ONCE))
        {
            ERROR(__FUNCTION__ << " | failed event_base_loop");
        }
    }
}
//------------------------------------------------------------------

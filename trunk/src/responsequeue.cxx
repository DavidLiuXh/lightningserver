#include "responsequeue.hxx"

#include "util/logger.hxx"

#include <string.h>
#include <errno.h>

using namespace Lightning;
//-------------------------------------------------
ResponseQueue::ResponseQueue(EventBaseWeakPtrType eventBase)
        :mEventBase(eventBase)
{
    ::memset(&mFdPair, 0, sizeof(mFdPair));
}

ResponseQueue::~ResponseQueue()
{

}
//-------------------------------------------------
bool ResponseQueue::init()
{
    return initFdPair();
}

void ResponseQueue::uninit()
{
    uninitFdPair();
}

bool ResponseQueue::push(ProcessQueueItem* item)
{
    bool rt = false;

    if (NULL != item)
    {
        ProcessQueue::push(item);
        rt = notifyResponsePush();
    }

    return rt;
}
//-------------------------------------------------
bool ResponseQueue::notifyResponsePush()
{
    bool rt = true;

    char data[1] = {0};
    if (-1 == ::send(mFdPair[0],
                data,
                sizeof(data)/sizeof(data[0]),
                0))
    {
        rt = false;
        ERROR(__FUNCTION__ << " | failed to send"
                    << " | errno : "  << errno
                    << " | msg : " << strerror(errno));
    }

    return rt;
}

bool ResponseQueue::recvResponsePushNotify()
{
    bool rt = false;

    char recvData[1];
    if (0  < ::recv(mFdPair[1], recvData, sizeof(recvData)/sizeof(recvData[0]), 0))
    {
        rt = true;
    }
    else
    {
        ERROR(__FUNCTION__ << " | failed to recv"
                    << " | errno : "  << errno
                    << " | msg : " << strerror(errno));
    }

    return rt;
}

bool ResponseQueue::initFdPair()
{
    bool rt = false;

    if (0 == socketpair(AF_UNIX, SOCK_STREAM, 0, mFdPair))
    {
        boost::shared_ptr<event_base> eventBase = mEventBase.lock();
        if (eventBase)
        {
            event* sockEvent = event_new(eventBase.get(),
                        mFdPair[1],
                        EV_READ | EV_PERSIST,
                        ResponseQueue::onResponsePushNotify,
                        this);
            if (NULL != sockEvent &&
                        0 == event_add(sockEvent, NULL))
            {
                rt = true;
            }
            else
            {
                ERROR(__FUNCTION__ << " | invalid arguments");
                close(mFdPair[0]);
                close(mFdPair[1]);
            }
        }
    }
    else
    {
        ERROR(__FUNCTION__ << " | failed to socketpair"
                    << " | errno : "  << errno
                    << " | msg : " << strerror(errno));
    }

    return rt;
}

void ResponseQueue::uninitFdPair()
{
    if (0 != mFdPair[0])
    {
        close(mFdPair[0]);
    }
    
    if (0 != mFdPair[1])
    {
        close(mFdPair[1]);
    }
}

void ResponseQueue::onResponsePushNotify(evutil_socket_t fd,
            short events,
            void* userData)
{
    ResponseQueue* me = static_cast<ResponseQueue*>(userData);
    if (NULL != me)
    {
        me->recvResponsePushNotify();
        me->OnResponsePushNotify(me);
    }
}
//-------------------------------------------------

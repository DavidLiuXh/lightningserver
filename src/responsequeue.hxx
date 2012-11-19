#ifndef LIGHTNING_RESPONSE_QUEUE_HXX
#define LIGHTNING_RESPONSE_QUEUE_HXX

#include "processqueue.hxx"

#include <event.h>

#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
//-------------------------------------------------
namespace Lightning
{
    class ResponseQueue
        :public ProcessQueue
    {
        private:
            typedef boost::function<void (ResponseQueue*)> ResponsePushNotifyEvent;

        public:
            typedef boost::weak_ptr<event_base> EventBaseWeakPtrType;

        public:
            ResponseQueue(EventBaseWeakPtrType eventBase);
            ~ResponseQueue();

        public:
            bool push(ProcessQueueItem* item);
            bool init();
            void uninit();

        public:
            ResponsePushNotifyEvent OnResponsePushNotify;

        private:
            bool initFdPair();
            void uninitFdPair();

            bool notifyResponsePush();
            bool recvResponsePushNotify();
            static void onResponsePushNotify(evutil_socket_t fd,
                        short events,
                        void* userData);
        private:
            EventBaseWeakPtrType mEventBase;
            int mFdPair[2];
    };//class ResponseQueue
}//namespace Lightning
//-------------------------------------------------
#endif//#ifndef LIGHTNING_RESPONSE_QUEUE_HXX

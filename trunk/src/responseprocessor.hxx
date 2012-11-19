#ifndef LIGHTNING_RESPONSE_PROCESSOR_HXX
#define LIGHTNING_RESPONSE_PROCESSOR_HXX

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
//------------------------------------------------------
struct event_base;

namespace Lightning
{
    class ResponseQueue;
    class ProcessQueueItem;

    class ResponseProcessor
    {
        private:
            typedef boost::shared_ptr<ProcessQueueItem> ProcessQueueItemPtrType;
            typedef boost::function<void (const ProcessQueueItemPtrType&)> ResponseNotifyEvent;

        public:
            typedef boost::weak_ptr<event_base> EventBaseWeakPtrType;

        public:
            ResponseProcessor(EventBaseWeakPtrType eventBase);
            ~ResponseProcessor();

        public:
            void start();
            void stop();
            void addResponse(ProcessQueueItem* response); 

        public:
            ResponseNotifyEvent OnProcessResponse;

        private:
            void process();
            void onResponsePushNotify(ResponseQueue*);

        private:
            boost::shared_ptr<ResponseQueue> mProcessQueue;
            EventBaseWeakPtrType mEventBase;
            bool mStop;
    };//class ResponseProcessor
}//namespace Lightning
//------------------------------------------------------
#endif//#ifndef LIGHTNING_RESPONSE_PROCESSOR_HXX

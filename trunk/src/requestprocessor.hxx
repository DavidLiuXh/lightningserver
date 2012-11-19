#ifndef LIGHTNING_REQUEST_PROCESSOR_HXX
#define LIGHTNING_REQUEST_PROCESSOR_HXX

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/condition.hpp>
#include <boost/threadpool.hpp>
#include <boost/function.hpp>
//--------------------------------------------------
namespace boost
{
    class thread;
}

namespace Lightning
{
    class ProcessQueue;
    class ProcessQueueItem;
    struct SessionInfo;

    class RequestProcessor 
    {
        private:
            typedef boost::shared_ptr<ProcessQueueItem> ProcessQueueItemPtrType;

        public:
            boost::function<void (RequestProcessor&, const ProcessQueueItemPtrType&)> SessionRequestEvent;

        public:
            RequestProcessor(size_t workThreadSize);
            ~RequestProcessor();

        public:
            void start();
            void stop();

            void addRequest(ProcessQueueItem* request);

        private:
            void initThreadPool();
            void run();
            void addTask(ProcessQueueItemPtrType requestTask);
            void processTask(ProcessQueueItemPtrType requestTask);

        private:
            boost::shared_ptr<ProcessQueue> mProcessQueue;
            boost::shared_ptr<boost::threadpool::pool> mTaskExcutor;
            boost::scoped_ptr<boost::thread> mThread;
            boost::mutex mMutex;
            boost::condition mCondition;
            
            size_t mWorkThreadSize;
            bool mShutdown;
    };//RequestProcessor 
}//namespace Lightning
//--------------------------------------------------
#endif//#ifndef LIGHTNING_REQUEST_PROCESSOR_HXX

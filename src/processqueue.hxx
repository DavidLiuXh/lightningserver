#ifndef LIGHTNING_PROCESS_QUEUE_HXX
#define LIGHTNING_PROCESS_QUEUE_HXX

#include <boost/shared_ptr.hpp>
#include <boost/lockfree/fifo.hpp>
//-------------------------------------------------
namespace Lightning
{
    class ProcessQueueItem;

    typedef boost::shared_ptr<ProcessQueueItem> ProcessQueueItemPtrType;

    class ProcessQueue
    {
        private:
            typedef boost::lockfree::fifo<ProcessQueueItem*> ProcessItemQueueType;

        public:
            ProcessQueue();
            ~ProcessQueue();
            
        public:
            bool push(ProcessQueueItem* item);
            bool isEmpty() { return mProcessItems.empty(); }
            ProcessQueueItemPtrType pop();

        protected:
            ProcessItemQueueType mProcessItems;
    };//class ProcessQueue
}//namespace Lightning
//-------------------------------------------------
#endif//#ifndef LIGHTNING_PROCESS_QUEUE_HXX

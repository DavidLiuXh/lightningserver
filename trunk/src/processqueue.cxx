#include "processqueue.hxx"
#include "request.hxx"

#include "util/logger.hxx"

using namespace Lightning;
//-------------------------------------------------
ProcessQueue::ProcessQueue()
{
    DEBUG(__FUNCTION__
                << " | is lock free : "
                << mProcessItems.is_lock_free());
}

ProcessQueue::~ProcessQueue()
{

}
//-------------------------------------------------
bool ProcessQueue::push(ProcessQueueItem* item)
{
    return mProcessItems.enqueue(item);
}

ProcessQueueItemPtrType ProcessQueue::pop()
{
    ProcessQueueItemPtrType item;

    mProcessItems.dequeue(item);

    return item;
}
//-------------------------------------------------

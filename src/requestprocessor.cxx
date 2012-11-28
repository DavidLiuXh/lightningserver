#include "requestprocessor.hxx"

#include "processqueue.hxx"

#include "util/logger.hxx"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace Lightning;
//----------------------------------------------------------
RequestProcessor::RequestProcessor(size_t workThreadSize)
:mProcessQueue(new ProcessQueue)
,mWorkThreadSize(workThreadSize)
,mShutdown(false)
{

}

RequestProcessor::~RequestProcessor()
{
}
//----------------------------------------------------------
void RequestProcessor::start()
{
    INFO(__FUNCTION__ << " | request processor start");
    initThreadPool();
    mThread.reset(new boost::thread(boost::bind(
                        &RequestProcessor::run,
                        this))); 
}

void RequestProcessor::stop()
{
    INFO(__FUNCTION__ << " | request processor stopping");

    if (mThread)
    {
        mShutdown = true;

        {
            boost::mutex::scoped_lock(mMutext);
            mCondition.notify_one();
        }

        mThread->join();

        INFO(__FUNCTION__ << " | request processor stop finished");

        if (mTaskExcutor)
        {
            mTaskExcutor->wait();
        }
    }

    INFO(__FUNCTION__ << " | TaskExcutor finished");
}

void RequestProcessor::addRequest(ProcessQueueItem* request)
{
    if (!mShutdown &&
                NULL != request &&
                mProcessQueue)
    {
        mProcessQueue->push(request);

        boost::mutex::scoped_lock(mMutext);
        mCondition.notify_one();
    }
}
//----------------------------------------------------------
void RequestProcessor::initThreadPool()
{
    mTaskExcutor.reset(new boost::threadpool::pool(mWorkThreadSize));
    assert(mTaskExcutor);
}

void RequestProcessor::run()
{
    while (!mShutdown)
    {
        {
            boost::mutex::scoped_lock lock(mMutex);
            while (!mShutdown &&
                        mProcessQueue->isEmpty())
            {
                mCondition.wait(lock);
            }
        }

        while (!mShutdown &&
                    !mProcessQueue->isEmpty())
        {
            ProcessQueueItemPtrType request = mProcessQueue->pop();
            if (request)
            {
                addTask(request);
            }
        }
    }
}

void RequestProcessor::addTask(ProcessQueueItemPtrType requestTask)
{
    if (mTaskExcutor)
    {
        mTaskExcutor->schedule(boost::bind(
                        &RequestProcessor::processTask,
                        this,
                        requestTask));
    }
}

void RequestProcessor::processTask(ProcessQueueItemPtrType requestTask)
{
    DEBUG(__FUNCTION__);
    if (!SessionRequestEvent.empty())
    {
        SessionRequestEvent(*this, requestTask);
    }
}
//----------------------------------------------------------

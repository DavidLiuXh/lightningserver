#include "responseprocessor.hxx"

#include "responsequeue.hxx"

#include "util/logger.hxx"

#include <event.h>

#include <boost/bind.hpp>

using namespace Lightning;
//-----------------------------------------------
ResponseProcessor::ResponseProcessor(EventBaseWeakPtrType eventBase)
        :mEventBase(eventBase)
        ,mStop(false)
{

}

ResponseProcessor::~ResponseProcessor()
{

}
//-----------------------------------------------
void ResponseProcessor::start()
{
    INFO(__FUNCTION__ << " | start");

    mProcessQueue.reset(new ResponseQueue(mEventBase));
    if (mProcessQueue)
    {
        mProcessQueue->OnResponsePushNotify = boost::bind(
                    &ResponseProcessor::onResponsePushNotify,
                    this,
                    _1) ;
        mProcessQueue->init();
    }
}

void ResponseProcessor::stop()
{
    INFO(__FUNCTION__ << " | stop");

    mStop = true;

    if (mProcessQueue)
    {
        mProcessQueue->OnResponsePushNotify.clear();
        mProcessQueue->uninit();
    }
}

void ResponseProcessor::addResponse(ProcessQueueItem* response)
{
    INFO(__FUNCTION__);

    if (mProcessQueue)
    {
        mProcessQueue->push(response);
    }
} 
//-----------------------------------------------
void ResponseProcessor::process()
{
    while (!mStop &&
                !mProcessQueue->isEmpty())
    {
        ProcessQueueItemPtrType responseTask = mProcessQueue->pop();
        if (responseTask)
        {
            OnProcessResponse(responseTask);
        }
    }
}

void ResponseProcessor::onResponsePushNotify(ResponseQueue*)
{
    process();
}
//-----------------------------------------------

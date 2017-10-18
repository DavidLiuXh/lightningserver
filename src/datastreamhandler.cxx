#include "datastreamhandler.hxx"

#include "userrequestwithheader.hxx"

#include "util/logger.hxx"

#include <boost/scoped_array.hpp>
using namespace Lightning;
//-----------------------------------------------
DataStreamHandler::DataStreamHandler()
:mHeaderSize(0)
,mBodySize(0)
,mStep(PS_HEADER)
{
    initEvBuffer();
}
//-----------------------------------------------
void DataStreamHandler::setUserRequest(boost::shared_ptr<UserRequest> request)
{
    mUserRequest = boost::dynamic_pointer_cast<UserRequestWithHeader>(request);
    if (mUserRequest)
    {
        mHeaderSize = mUserRequest->getHeaderSize();
    }
}

void DataStreamHandler::pushData(evbuffer* buffer)
{
    bool needContinue = true;

    evbuffer_remove_buffer(buffer, mCache.get(), evbuffer_get_length(buffer));

    while (needContinue)
    {
        switch (mStep)
        {
            case PS_HEADER:
                {
                    if (!recvHeader())
                    {
                        DEBUG(__FUNCTION__ << " | cotinue recv header");
                        needContinue = false;
                    }
                    else if (recvBody())
                    {
                        DEBUG(__FUNCTION__ << " | ok to recv body");
                        needContinue = true;
                    }
                    else
                    {
                        DEBUG(__FUNCTION__ << " | ok to recv header and cotinue recv body");
                        mStep = PS_BODY;
                        needContinue = false;
                    }
                    break;
                }
            case PS_BODY:
                {
                    if (recvBody())
                    {
                        DEBUG(__FUNCTION__ << " | ok to recv body");
                        mStep = PS_HEADER;
                        needContinue = true;
                    }
                    else
                    {
                        DEBUG(__FUNCTION__ << " | continue recv body");
                        needContinue = false;
                    }
                }
                break;
        }
    }
}


bool DataStreamHandler::recvHeader()
{
    bool needContinue = true;

    size_t totalSize = evbuffer_get_length(mCache.get());
    DEBUG(__FUNCTION__ << " | current total size of evbuufer : " << totalSize);
    if (totalSize < mHeaderSize)
    {
        needContinue = false;
    }
    else
    {
        boost::scoped_array<char> header(new char[mHeaderSize]);
        evbuffer_remove(mCache.get(),
                    header.get(),
                    mHeaderSize);
        mUserRequest->setHeader(header.get(), mHeaderSize);
        mBodySize = mUserRequest->getBodySize();
    }

    return needContinue;
}

bool DataStreamHandler::recvBody()
{
    bool needContinue = false;

    size_t totalSize = evbuffer_get_length(mCache.get());
    DEBUG(__FUNCTION__ << " | current total size of evbuufer : " << totalSize);
    if (totalSize >= mBodySize)
    {
        boost::scoped_array<char> body(new char[mBodySize]);
        evbuffer_remove(mCache.get(),
                    body.get(),
                    mBodySize);
        mUserRequest->setBody(body.get(), mBodySize);
        if (!OnRecvRequestFinished.empty())
        {
            OnRecvRequestFinished(this, mUserRequest);
        }
        needContinue = true;
    }

    return needContinue;
}
//-----------------------------------------------
struct FreeEvBuffer
{
    void operator()(evbuffer* buffer)
    {
        if (NULL != buffer)
        {
            evbuffer_free(buffer);
        }
    }
};

void DataStreamHandler::initEvBuffer()
{
    mCache.reset(evbuffer_new(), FreeEvBuffer());
}
//-----------------------------------------------

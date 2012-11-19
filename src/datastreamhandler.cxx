#include "datastreamhandler.hxx"

#include "userrequest.hxx"

#include "util/logger.hxx"

#include <boost/scoped_array.hpp>
using namespace Lightning;
//-----------------------------------------------
DataStreamHandler::DataStreamHandler(boost::shared_ptr<UserRequest> request)
:mHeaderSize(0)
,mBodySize(0)
,mStep(PS_HEADER)
,mUserRequest(boost::shared_dynamic_cast<UserRequestWithHeader>(request))
{
    if (mUserRequest)
    {
        mHeaderSize = mUserRequest->getHeaderSize();
    }
    initEvBuffer();
}
//-----------------------------------------------
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
                        needContinue = false;
                    }
                    else if (recvBody())
                    {
                        needContinue = true;
                    }
                    else
                    {
                        mStep = PS_BODY;
                        needContinue = false;
                    }
                    break;
                }
            case PS_BODY:
                {
                    if (recvBody())
                    {
                        mStep = PS_HEADER;
                        needContinue = true;
                    }
                    else
                    {
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
    if (totalSize >= mBodySize)
    {
        boost::scoped_array<char> body(new char[mBodySize]);
        evbuffer_remove(mCache.get(),
                    body.get(),
                    mBodySize);
        mUserRequest->setBody(body.get(), mBodySize);
        OnRecvRequestFinished(this, mUserRequest);
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

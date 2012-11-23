#include "datalinehandler.hxx"

#include "userrequestwithline.hxx"

#include "util/strutil.hxx"
#include "util/logger.hxx"

using namespace Lightning;
//------------------------------------------
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
//-----------------------------------------
DataLineHandler::DataLineHandler()
{
    mCache.reset(evbuffer_new(), FreeEvBuffer());
}
//-----------------------------------------
void DataLineHandler::setUserRequest(boost::shared_ptr<UserRequest> request)
{
    mUserRequest = boost::shared_dynamic_cast<UserRequestWithLine>(request);
}

void DataLineHandler::pushData(evbuffer* buffer)
{
    evbuffer_remove_buffer(buffer, mCache.get(), evbuffer_get_length(buffer));
    
    char* readLine = NULL;
    size_t readCount = 0;
    while (true)
    {
        readLine = evbuffer_readln(mCache.get(),
                    &readCount,
                    EVBUFFER_EOL_CRLF);
        if (NULL == readLine)
        {
            DEBUG(__FUNCTION__ << " | " << readCount);
            break;
        }
        else
        {
            DEBUG(__FUNCTION__ << " | input length" << readCount);
            mUserRequest->setBody(readLine, readCount);
            OnRecvRequestFinished(this, mUserRequest);
        }
    }
}
//------------------------------------------

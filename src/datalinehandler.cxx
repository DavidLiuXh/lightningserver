#include "datalinehandler.hxx"

#include "userrequestwithline.hxx"

#include "util/strutil.hxx"

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
DataLineHandler::DataLineHandler(boost::shared_ptr<UserRequest> request)
        :mUserRequest(boost::shared_dynamic_cast<UserRequestWithLine>(request))
        ,mEofFlag(LUtil::StrUtil::sEmpty)
{
    mCache.reset(evbuffer_new(), FreeEvBuffer());
}
//-----------------------------------------
void DataLineHandler::setUserRequest(boost::shared_ptr<UserRequest> request)
{
    mUserRequest = boost::shared_dynamic_cast<UserRequestWithLine>(request);
    if (mUserRequest)
    {
        const char* eofFlag = mUserRequest->getEofFlag();
        if (LUtil::StrUtil::notEmptyOrNull(eofFlag))
        {
            mEofFlag = eofFlag;
        }
    }
}

void DataLineHandler::pushData(evbuffer* buffer)
{
    evbuffer_remove_buffer(buffer, mCache.get(), evbuffer_get_length(buffer));
    
    bool needContinue = true;
    evbuffer_ptr found;
    while (needContinue)
    {
        found = evbuffer_search(mCache.get(),
                    mEofFlag.c_str(),
                    mEofFlag.length(),
                    NULL);
    }
}
//------------------------------------------

#include "../src/LightningServer.hxx"
#include "../src/sessionHandler.hxx"
#include "../src/userrequestwithheader.hxx"
#include "../src/userrequestwithline.hxx"
#include "../src/userresponse.hxx"
#include "../util/logger.hxx"

#include <boost/scoped_array.hpp>

#include <google/profiler.h>

#include <string.h>
#include <string>

using namespace Lightning;
//----------------------------------------------------
class MySessionHandler
:public SessionHandler
{
    public:
        void onStart(int sid, const SessionInfo& sessionInfo)
        {
            INFO(__FUNCTION__ << " | fire onStart : " << sid);
        }
        
        void onRequest(int sid, const boost::shared_ptr<UserRequest>& request,
                    boost::shared_ptr<UserResponse>& response,
                    bool& isContinue)
        {
            //isContinue = false;

            std::string data("Hello World");
            if (response)
            {
                response->write(data.c_str(), data.length() + 1);
            }
            INFO(__FUNCTION__ << " | request : " << request->toString().length());
        }
        
        virtual void onEnded(int sid)
        {
            INFO(__FUNCTION__ << " | sid : " << sid);
        }
        
        virtual void onError(int sid, SessionErrorCode code)
        {
            INFO(__FUNCTION__ << " | sid : " << sid
                        << " | error code : " << code);
        }
};

class MyRequest
:public UserRequestWithHeader 
{
    public:
        MyRequest(size_t headerSize)
            :mHeaderSize(headerSize)
            ,mBodySize(0)
        {
        }

    public:
        virtual size_t getHeaderSize() const
        {
            return mHeaderSize;
        }

        virtual bool setHeader(const char* header, size_t length)
        {
            mHeader.reset(new char[length]);
            memcpy(mHeader.get(), header, length);
            memcpy((void*)(&mBodySize), header, length);
            //DEBUG(__FUNCTION__ << " | BODY SIZE : " << mBodySize);
            return true;
        }

        virtual bool setBody(const char* body, size_t length)
        {
            mBody.reset(new char[length]);
            memcpy(mBody.get(), body, length);
            return true;
        }

        virtual size_t getBodySize() const
        {
            return mBodySize;
        }

        virtual std::string toString()
        {
            return std::string(mBody.get(), mBodySize);
        }

    private:
        typedef boost::scoped_array<char> BuffreType;

    private:
        size_t mHeaderSize;
        int mBodySize;
        BuffreType mHeader;
        BuffreType mBody;
};

class MyRequestFactory
:public UserRequestWithHeaderFactory
{
    public:
        virtual UserRequestPtrType create()
        {
            return UserRequestPtrType(new MyRequest(4));
        }
};

class MyResponse
:public UserResponse
{
    public:
        virtual void write(const char* data, size_t length)
        {
            mData.resize(length);;
            memcpy(&mData[0], data, length);
        }

        virtual const char* serialization(size_t& length)
        {
            length = mData.size();
            return &mData[0];
        }
    private:
        std::vector<char> mData;
};

class MyResponseFactory
:public UserResponseFactory
{
    public:
        virtual UserResponsePtrType create()
        {
            return UserResponsePtrType(new MyResponse);
        }
};

class MyLineRequest
:public UserRequestWithLine
{
    public:
        virtual void setBody(const char* body, size_t length)
        {
            std::string tmp(body, length);
            mBody += tmp;
        }

        virtual std::string toString()
        {
            return mBody;
        }
    private:
        std::string mBody;
};

class MyLineRequestFactory
:public UserRequestWithLineFactory
{
    public:
        virtual UserRequestPtrType create()
        {
            return UserRequestPtrType(new MyLineRequest);
        }
};

int main(int argc, char* argv[])
{
    ProfilerStart("./cpu");

    boost::shared_ptr<UserRequestFactory> userRequestFactory;
    if (0 == strcmp(argv[1], "withline"))
    {
        userRequestFactory.reset(new MyLineRequestFactory);
    }
    else// if (0 == strcmp(argv[1], "withheader"))
    {
        userRequestFactory.reset(new MyRequestFactory);
    }

    LightningServer::SessionHandlerPtrType mySessionHandler(
                new MySessionHandler());
    LightningServer ls(userRequestFactory,
                boost::shared_ptr<UserResponseFactory>(new MyResponseFactory),
                false,
                "./test.log");
    ls.regSessionHandler(mySessionHandler);
    ls.start(NULL, 6666);

    ProfilerStop();
    return 0;
}
//----------------------------------------------------

#ifndef LIGHTNING_USER_REQUEST_WITH_LINE_HXX
#define LIGHTNING_USER_REQUEST_WITH_LINE_HXX

#include "userrequest.hxx"

namespace Lightning
{
    class UserRequestWithLine
        :public UserRequest
    {
        public:
           virtual ~UserRequestWithLine(){}

        public:
           virtual RequestType getType() { return RT_STREAM_WITH_LINE; }
           virtual void setBody(const char* body, size_t length) = 0;

    };//class UserRequestWithLinet
    
    typedef boost::shared_ptr<UserRequestWithLine> UserRequestWithLinePtrType;

    class UserRequestWithLineFactory
        :public UserRequestFactory
    {
        public:
            virtual ~UserRequestWithLineFactory() {}
            virtual UserRequestPtrType create() = 0;
    };//class UserRequestWithLineFactory
    
    typedef boost::shared_ptr<UserRequestWithLineFactory> UserRequestWithLineFactoryPtrType;

}//namespace Lightning
#endif//#ifndef LIGHTNING_USER_REQUEST_WHIT_LENG_HXX

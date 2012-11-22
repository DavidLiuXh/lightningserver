#ifndef LIGHTNING_USER_REQUEST_WITH_HEADER_HXX
#define LIGHTNING_USER_REQUEST_WITH_HEADER_HXX

#include "userrequest.hxx"
//--------------------------------------------------
namespace Lightning
{
    class UserRequestWithHeader
        :public UserRequest
    {
        public:
            virtual ~UserRequestWithHeader() {}
            
        public:
            virtual RequestType getType() { return RT_STREAM_WITH_HEADER; }
            virtual size_t getHeaderSize() const = 0;
            virtual bool setHeader(const char* header, size_t length) = 0;
            virtual bool setBody(const char* body, size_t length) = 0;
            virtual size_t getBodySize() const = 0;
    };
    
    typedef boost::shared_ptr<UserRequestWithHeader> UserRequestWithHeaderPtrType;

    class UserRequestWithHeaderFactory
        :public UserRequestFactory
    {
        public:
            virtual ~UserRequestWithHeaderFactory() {}
            virtual UserRequestPtrType create() = 0;
    };//class UserRequestFactory
    
    typedef boost::shared_ptr<UserRequestFactory> UserRequestFactoryPtrType;
}//namespace Lightning
#endif//#ifndef LIGHTNING_USER_REQUEST_WITH_HEADER_HXX

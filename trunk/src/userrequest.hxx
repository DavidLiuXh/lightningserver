#ifndef LIGHTNGING_USERREQUEST_HXX
#define LIGHTNGING_USERREQUEST_HXX

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
//--------------------------------------------
namespace Lightning
{
    class UserRequest
    {
        public:
            virtual ~UserRequest() {}
            virtual std::string toString() = 0;
        public:
    };//class UserRequest

    typedef boost::shared_ptr<UserRequest> UserRequestPtrType;

    class UserRequestFactory
    {
        public:
            virtual ~UserRequestFactory() {}
            virtual UserRequestPtrType create() = 0;
    };//class UserRequestFactory
    
    typedef boost::shared_ptr<UserRequestFactory> UserRequestFactoryPtrType;
    //--------------------------------------------------
    class UserRequestWithHeader
        :public UserRequest
    {
        public:
            virtual ~UserRequestWithHeader() {}
            
        public:
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
//--------------------------------------------
#endif//#ifndef LIGHTNGING_USERREQUEST_HXX

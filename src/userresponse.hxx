#ifndef LIGHTNGING_USERRESPONSE_HXX
#define LIGHTNGING_USERRESPONSE_HXX

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
//--------------------------------------------
namespace Lightning
{
    class UserResponse
    {
        public:
            virtual ~UserResponse() {}

        public:
            virtual void write(const char* data, size_t length) = 0;
            virtual const char* serialization(size_t& length) = 0;
    };//class UserRequest

    typedef boost::shared_ptr<UserResponse> UserResponsePtrType;

    class UserResponseFactory
        :boost::noncopyable
    {
        public:
            virtual ~UserResponseFactory() {}
            virtual UserResponsePtrType create() = 0;
    };//class UserResponseFactory

    typedef boost::shared_ptr<UserResponseFactory> UserResponseFactoryPtrType;
}//namespace Lightning
//--------------------------------------------
#endif//#ifndef LIGHTNGING_USERRESPONSE_HXX

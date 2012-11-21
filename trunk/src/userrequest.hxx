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
}//namespace Lightning
//--------------------------------------------
#endif//#ifndef LIGHTNGING_USERREQUEST_HXX

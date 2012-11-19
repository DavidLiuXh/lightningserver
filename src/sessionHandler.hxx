#ifndef LIGHTNING_SESSION_HANDLER_HXX
#define LIGHTNING_SESSION_HANDLER_HXX

#include "errorcode.hxx"

#include <boost/shared_ptr.hpp>
//----------------------------------------------
namespace Lightning
{
    class SessionInfo;
    class UserRequest;
    class UserResponse;
    enum SessionErrorCode;

    class SessionHandler
    {
        public:
            virtual ~SessionHandler() {}

        public:
            virtual void onStart(int sid, const SessionInfo& sessionInfo) = 0;
            virtual void onRequest(int sid, const boost::shared_ptr<UserRequest>& request,
                        boost::shared_ptr<UserResponse>& response) = 0;
            virtual void onEnded(int sid) = 0;
            virtual void onError(int sid, SessionErrorCode code) = 0;
    };
}//namespace Lightning
//----------------------------------------------
#endif//#ifndef LIGHTNING_SESSION_HANDLER_HXX

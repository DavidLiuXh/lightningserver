#ifndef LIGHTNING_SESSION_MANAGER_HXX
#define LIGHTNING_SESSION_MANAGER_HXX

#include <boost/shared_ptr.hpp>

#include <set>
//---------------------------------------------
namespace Lightning
{
    class Session;

    class SessionManager
    {
        public:
            typedef boost::shared_ptr<Session> SessionPtrType;
            typedef std::set<SessionPtrType> SessionContainerType;
        public:
            SessionManager();
            ~SessionManager();

        public:
            bool addSession(SessionPtrType session);
            void removedSession(SessionPtrType session);
            
        private:
            SessionContainerType mSessionContainer;
    };
}//namespace Lightning
//---------------------------------------------
#endif//#ifndef LIGHTNING_SESSION_MANAGER_HXX

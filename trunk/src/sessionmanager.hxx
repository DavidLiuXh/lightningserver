#ifndef LIGHTNING_SESSION_MANAGER_HXX
#define LIGHTNING_SESSION_MANAGER_HXX

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/functional/hash/extensions.hpp>

#include <set>
//---------------------------------------------
namespace Lightning
{
    class Session;
    struct SessionEntry;

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
            void updateSession(boost::weak_ptr<SessionEntry> session);
            void freshTimeWheel();
            size_t getSessionCount() const;
            
        private:
            class Entry;

            typedef boost::shared_ptr<SessionEntry> EntryPtrType;
            typedef boost::unordered_set<EntryPtrType> Bucket;
            typedef boost::circular_buffer<Bucket> SessionTimeoutListType;

        private:
            SessionContainerType mSessionContainer;
            SessionTimeoutListType mSesstionTimeoutList;
    };
}//namespace Lightning
//---------------------------------------------
#endif//#ifndef LIGHTNING_SESSION_MANAGER_HXX

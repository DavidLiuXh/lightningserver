#include "sessionmanager.hxx"

#include "session.hxx"
#include "sessionentry.hxx"

#include "util/logger.hxx"

using namespace Lightning;

typedef boost::weak_ptr<SessionEntry> EntryWeakPtrType;
//------------------------------------------
SessionManager::SessionManager()
{
    mSesstionTimeoutList.resize(5);
}

SessionManager::~SessionManager()
{
    mSessionContainer.clear();
}
//------------------------------------------
bool SessionManager::addSession(SessionPtrType session)
{
   bool rt = false;

   if (session)
   {
       DEBUG(__FUNCTION__ << " | "
                   << (*session));
       rt = mSessionContainer.insert(session).second;
       if (!rt)
       {
           ERROR(__FUNCTION__ << " | already exist");
       }

#if 1
       EntryPtrType entry(new SessionEntry(session));
       if (entry)
       {
           session->setSessionEntry(entry);
           mSesstionTimeoutList.back().insert(entry);
       }
#endif
   }

   return rt;
}

void SessionManager::freshTimeWheel()
{
    mSesstionTimeoutList.push_back(Bucket());
}

void SessionManager::updateSession(EntryWeakPtrType sessionEntry)
{
    DEBUG(__FUNCTION__);

    EntryPtrType entry = sessionEntry.lock();
    if (entry)
    {
        mSesstionTimeoutList.back().insert(entry);
    }
}

void SessionManager::removedSession(SessionPtrType session)
{
    SessionContainerType::iterator found = mSessionContainer.find(session);
    if (mSessionContainer.end() != found)
    {
        mSessionContainer.erase(found);
    }
}
//------------------------------------------

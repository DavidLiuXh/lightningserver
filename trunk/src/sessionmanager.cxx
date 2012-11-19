#include "sessionmanager.hxx"

#include "session.hxx"

#include "util/logger.hxx"

using namespace Lightning;
//------------------------------------------
SessionManager::SessionManager()
{

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
   }

   return rt;
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

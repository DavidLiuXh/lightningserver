#include "sessionentry.hxx"
#include "session.hxx"

#include "util/logger.hxx"

using namespace Lightning;
//--------------------------------------------
SessionEntry::SessionEntry(const boost::shared_ptr<Session>& session)
:mSession(session)
{
}

SessionEntry::~SessionEntry()
{
    DEBUG(__FUNCTION__);
    boost::shared_ptr<Session> session = mSession.lock();
    if (session)
    {
        session->shutdown();
    }
}
//--------------------------------------------

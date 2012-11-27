#ifndef LIGHTNING_SESSION_ENTRY_HPP
#define LIGHTNING_SESSION_ENTRY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
//---------------------------------------------------------
namespace Lightning
{
    class Session;

    struct SessionEntry
        :public boost::noncopyable 
    {
        typedef boost::weak_ptr<Session> SessionWeakPtrType;

        SessionEntry(const boost::shared_ptr<Session>& session);
        ~SessionEntry();

        SessionWeakPtrType mSession;
    };
}//namespace Lightning
#endif//#ifndef LIGHTNING_SESSION_ENTRY_HPP

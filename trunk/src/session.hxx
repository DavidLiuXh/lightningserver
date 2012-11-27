#ifndef LIGHTNING_SESSION_HXX
#define LIGHTNING_SESSION_HXX

#include "errorcode.hxx"
#include "requesttype.hxx"

#include <event.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <iostream>
//--------------------------------------------------------
namespace Lightning
{
    struct SessionInfo;
    class DataHandler;
    class UserRequest;
    class UserRequestFactory;
    class SessionEntry;
    class SessionManager;
  
    class Session
        :public boost::enable_shared_from_this<Session>
    {
        private:
            typedef boost::shared_ptr<SessionEntry> SessionEntryPtrType;
            typedef int SessionIdType;
            typedef boost::shared_ptr<UserRequest> UserRequestPtrType;
            typedef boost::function<void (boost::shared_ptr<Session>, UserRequestPtrType)> RecvRequestFinishNotify;
            typedef boost::function<void (boost::shared_ptr<Session>)> ClientFdClosedNotify;
            typedef boost::function<void (boost::shared_ptr<Session>, Lightning::SessionErrorCode)> ErrorNotify;

        public:
            Session(boost::shared_ptr<SessionManager> sesionManager,
                        boost::weak_ptr<UserRequestFactory> mRequestFactory,
                        evutil_socket_t fd,
                        const char* ip);
            ~Session();

            std::ostream& toString(std::ostream& os) const;

        public:
            bool init(event_base* eb);
            void uninit();
            void shutdown();
            void startAction(short what);
            const SessionInfo& getInfo() const { return *mSessionInfo; }
            const SessionIdType getSessionId();
            void sendData(const char* data, size_t length);
            void setSessionEntry(const SessionEntryPtrType entry)
            {
                mSessionEntry = entry;
            }

        public:
            RecvRequestFinishNotify OnRecvRequestFinished;
            ClientFdClosedNotify OnClientFdClosed;
            ErrorNotify OnError;

        private:
            void initDataHandler();
            void onRecvRequestFinished(DataHandler*, UserRequestPtrType);

        private:
            class SessionUtil;

        private:
            boost::weak_ptr<SessionManager> mSessionManager;
            boost::weak_ptr<SessionEntry> mSessionEntry;
            boost::weak_ptr<UserRequestFactory> mUserRequestFactory;
            boost::shared_ptr<SessionInfo> mSessionInfo;
            boost::shared_ptr<DataHandler> mDataStreamHandler;
            boost::shared_ptr<bufferevent> mBufferEvent;
            UserRequestPtrType mUserRequest;
            SessionIdType mSessionId;
    };

    std::ostream& operator << (std::ostream& os, const Session& session);
}//namespace Lightning

//--------------------------------------------------------
#endif//#ifndef LIGHTNING_SESSION_HXX

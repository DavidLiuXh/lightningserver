#ifndef LIGHTNING_SESSION_HXX
#define LIGHTNING_SESSION_HXX

#include "errorcode.hxx"
#include <event.h>

#include <boost/shared_ptr.hpp>
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
    //enum SessionErrorCode;
  
    class Session
        :public boost::enable_shared_from_this<Session>
    {
        private:
            typedef int SessionIdType;
            typedef boost::shared_ptr<UserRequest> UserRequestPtrType;
            typedef boost::function<void (boost::shared_ptr<Session>, UserRequestPtrType)> RecvRequestFinishNotify;
            typedef boost::function<void (boost::shared_ptr<Session>)> ClientFdClosedNotify;
            typedef boost::function<void (boost::shared_ptr<Session>, Lightning::SessionErrorCode)> ErrorNotify;

        public:
            Session(evutil_socket_t fd, const char* ip);
            ~Session();

            std::ostream& toString(std::ostream& os) const;

        public:
            bool init(event_base* eb,
                        UserRequestPtrType userRequest);
            void uninit();
            void startAction(short what);
            const SessionInfo& getInfo() const { return *mSessionInfo; }
            const SessionIdType getSessionId();
            void sendData(const char* data, size_t length);

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

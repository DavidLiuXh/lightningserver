#ifndef LIGHTNING_SERVER_PROCESSOR_HXX
#define LIGHTNING_SERVER_PROCESSOR_HXX

#include "errorcode.hxx"

#include <event.h>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
//---------------------------------------------------
namespace Lightning
{
    class Session;
    class SessionHandler;
    class SessionManager;
    class ProcessQueueItem;
    class RequestProcessor;
    class ResponseProcessor;
    class UserRequest;
    class UserRequestFactory;
    class UserResponseFactory;

    class LightningServerProcessor
    {
        public:
            LightningServerProcessor(boost::shared_ptr<event_base> eb,
                        boost::shared_ptr<SessionHandler> sessionHandler,
                        boost::shared_ptr<UserRequestFactory> userReuqestFactory,
                        boost::shared_ptr<UserResponseFactory> userResponseFactory,
                        size_t maxSessionCount);

            ~LightningServerProcessor();
        public:
            void start();
            void stop();
            bool isRunning() const { return mRunning; }

            void processAccepted(evutil_socket_t fd);
            void processTimer(evutil_socket_t fd);

        private:
            void addSession(evutil_socket_t fd, sockaddr_in& addr);
            void startRequestProcessor();
            void stopRequestProcessor();
            void startResponseProcessor();
            void stopResponseProcessor();

            bool initSession(evutil_socket_t fd,
                        sockaddr_in& addr,
                        boost::shared_ptr<Session>& session);

            void handleRequestEvent(RequestProcessor& requestProcessor,
                        const boost::shared_ptr<ProcessQueueItem>& requestTask);
            void handleResponseEvent(const boost::shared_ptr<ProcessQueueItem>& requestTask);
            
            void onRecvRequestFinished(boost::shared_ptr<Session>, boost::shared_ptr<UserRequest> request);
            void onClientFdClosed(boost::shared_ptr<Session>);
            void onError(boost::shared_ptr<Session>, SessionErrorCode);

            void processRequestTask(const boost::shared_ptr<ProcessQueueItem>& requestTask);
            void processStartTask(const boost::shared_ptr<ProcessQueueItem>& requestTask);
            void processClientClosedTask(const boost::shared_ptr<ProcessQueueItem>& requestTask);
            void processError(const boost::shared_ptr<ProcessQueueItem>& requestTask);

            bool checkSessionCountToMax();
        private:
            boost::weak_ptr<event_base> mEventBase;
            boost::weak_ptr<SessionHandler> mSessionHandler;
            boost::weak_ptr<UserRequestFactory> mRequestFactory;
            boost::weak_ptr<UserResponseFactory> mResponseFactory;
            boost::shared_ptr<SessionManager> mSessionManager;
            boost::shared_ptr<ResponseProcessor> mResponseProcessor;
            boost::scoped_ptr<RequestProcessor> mRequestProcessor;

            bool mRunning;
            int mIdleFd;
            size_t mSessionMaxCount;
    };//class LightningServerProcessor
}//namespace Lightning

#endif//#ifndef LIGHTNING_SERVER_PROCESSOR_HXX

#ifndef LIGHTNING_SERVER_HXX
#define LIGHTNING_SERVER_HXX

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
//-------------------------------------------------
struct event_base;

namespace Lightning
{
    class SessionHandler;
    class LightningServerProcessor;
    class UserRequestFactory;
    class UserResponseFactory;

    class LightningServer
    {
        public:
            typedef boost::shared_ptr<SessionHandler> SessionHandlerPtrType;
            typedef boost::shared_ptr<UserRequestFactory> UserRequestFactoryPtrType;
            typedef boost::shared_ptr<UserResponseFactory> UserResponseFactoryPtrType;
        public:
            LightningServer(UserRequestFactoryPtrType userReuqestFactory,
                        UserResponseFactoryPtrType userResponseFactory,
                        const char* logPath);
            ~LightningServer();

        public:
            bool regSessionHandler(SessionHandlerPtrType sessionHandler);
            void start(const char* ip, int port);
            void stop();

        private:
            void initLog(const char* path);

            bool setupEventBase();
            void setupSignalHandler();
            bool startListen(const char* ip, int port);
            void process();
        private:
            UserRequestFactoryPtrType mRequestFactory;
            UserResponseFactoryPtrType mResponseFactory;
            SessionHandlerPtrType mSessionHandler;

            boost::shared_ptr<event_base> mEventBase;
            boost::scoped_ptr<LightningServerProcessor> mLSProcessor;

    };//class LightningServer

}//namespace Lightning

#endif//#ifndef LIGHTNING_SERVER_HXX

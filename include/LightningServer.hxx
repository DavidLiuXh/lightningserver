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
                        bool isDebugMode,
                        const char* logPath);
            ~LightningServer();

        public:
            inline const char* getVersion();

            bool regSessionHandler(SessionHandlerPtrType sessionHandler);
            void start(const char* ip, int port);
            void stop();

        private:
            class LightningServerImp;
            
        private:
            boost::scoped_ptr<LightningServerImp> mImp;

    };//class LightningServer

}//namespace Lightning

#endif//#ifndef LIGHTNING_SERVER_HXX

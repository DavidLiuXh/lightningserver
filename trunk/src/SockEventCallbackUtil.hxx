#ifndef LIGHTNING_SOCK_EVENT_CALLBACK_UTIL_HXX
#define LIGHTNING_SOCK_EVENT_CALLBACK_UTIL_HXX

#include <event.h>
//-------------------------------------------------------
namespace Lightning
{
    class SockEventCallbackUtil
    {
        public:
            static void onAccepted(evutil_socket_t, short, void *);
            static void onSignalHandler(evutil_socket_t, short, void *);
    };//class SockEventCallbackUtil
}//namespace Lightning
//-------------------------------------------------------
#endif//#ifndef LIGHTNING_SOCK_EVENT_CALLBACK_UTIL_HXX

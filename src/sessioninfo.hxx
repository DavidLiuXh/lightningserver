#ifndef LIGHTNING_SESSION_INFO_HXX
#define LIGHTNING_SESSION_INFO_HXX

#include <event.h>

#include <string>
//------------------------------------------
namespace Lightning
{
    struct SessionInfo
    {
        SessionInfo(evutil_socket_t fd,
                    const char* ip);

        evutil_socket_t mClientFd;
        std::string mClientIp;
    };
}//namespace Lightning
//------------------------------------------
#endif//#ifndef LIGHTNING_SESSION_INFO_HXX

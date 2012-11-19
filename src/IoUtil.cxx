#include "IoUtil.hxx"

#include "util/logger.hxx"
#include "util/strutil.hxx"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

using namespace Lightning;
//----------------------------------------------------------
bool IoUtil::setSockBlock(int fd, bool blocking)
{
    bool rt = false;

#ifdef WIN32
	unsigned long nonblocking = (blocking ? 0 : 1);
	rt = (0 == ioctlsocket(fd, FIONBIO, (unsigned long*)&nonblocking));
#else
	int flags = fcntl(fd, F_GETFL);
    if(flags >= 0 )
    {
        if (blocking)
        {
            flags &= ~O_NONBLOCK;
        }
        else
        {
            flags |= O_NONBLOCK;
        }

        if( fcntl( fd, F_SETFL, flags ) < 0 )
        {
            ERROR(__FUNCTION__ << " | fcntl failed"
                        << " | errno : "  << errno
                        << " | msg : " << strerror(errno));
        }
        else
        {
            rt = true;
        }
    }
#endif

	return rt;
}

bool IoUtil::setSockOptByIntValue(int fd,
            int level,
            int optName,
            bool use)
{
    bool rt = false;

    int flags = (use ? 1 : 0);
    if(setsockopt(fd,
                    level,
                    optName,
                    (void*)&flags,
                    sizeof(flags)) < 0)
    {
        ERROR(__FUNCTION__ << " | failed to setsockopt"
                    << " | level : " << level
                    << " | opt name : " << optName
                    << " | flags : " << flags);
    }
    else
    {
        rt = true;
    }

    return rt;
}

bool IoUtil::tcpListen(const char* ip,
            int port,
            bool blocking,
            int& fd)
{
    bool rt = false;

    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if( listenFd > 0 )
    {
        rt = true;
    }
    else
    {
        ERROR(__FUNCTION__ << " | socket failed"
                    << " | errno : "  << errno
                    << " | msg : " << strerror(errno));
    }

    if(rt && blocking)
    {
        rt = setSockBlock(listenFd, false);
    }

    if(rt)
    {
        setSockOptByIntValue(listenFd,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    true);

        setSockOptByIntValue(listenFd,
                    IPPROTO_TCP,
                    TCP_NODELAY,
                    true);
    }

    struct sockaddr_in addr;

    if(rt)
    {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons( port );

        addr.sin_addr.s_addr = INADDR_ANY;
        if(LUtil::StrUtil::notEmptyOrNull(ip))
        {
            if(0 == inet_aton(ip, &addr.sin_addr))
            {
                ERROR(__FUNCTION__ << " | failed to convert IP : " << ip);
                rt = false;
            }
        }
    }

    if(rt)
    {
        if(bind(listenFd,
                        (struct sockaddr*)&addr,
                        sizeof(addr)) < 0)
        {
            ERROR(__FUNCTION__ << " | bind failed"
                        << " | errno : " << errno
                        << " | msg : " << strerror(errno));
            rt = false;
        }
    }

    if(rt)
    {
        if(::listen(listenFd, 1024) < 0)
        {
            ERROR(__FUNCTION__ << " | listen failed"
                        << " | errno : " << errno
                        << " | msg : " << strerror(errno));
            rt = false;
        }
    }

    if(!rt &&
                listenFd >= 0)
    {
        close(listenFd);
    }

    if(rt)
    {
        fd = listenFd;
    }

    return rt;
}


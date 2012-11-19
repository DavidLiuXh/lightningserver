#ifndef LIGHTNING_IO_UTIL_HXX
#define LIGHTNING_IO_UTIL_HXX

#include <stddef.h>
//--------------------------------------------------------
namespace Lightning
{
    class IoUtil
    {
        public:
            static bool setSockBlock(int fd, bool blocking);
            static bool tcpListen(const char* ip,
                        int port,
                        bool blocking,
                        int& fd);
            static bool setSockOptByIntValue(int fd,
                        int level,
                        int optName,
                        bool use);
    };//class IoUtil
}
//--------------------------------------------------------
#endif//#ifndef LIGHTNING_IO_UTIL_HXX

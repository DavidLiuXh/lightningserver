#include "sessioninfo.hxx"

#include "util/strutil.hxx"

using namespace Lightning;
//-------------------------------------------
SessionInfo::SessionInfo(evutil_socket_t fd,
                    const char* ip)
:mClientFd(fd)
,mClientIp(LUtil::StrUtil::getSafeStr(ip))
{
}
//-------------------------------------------

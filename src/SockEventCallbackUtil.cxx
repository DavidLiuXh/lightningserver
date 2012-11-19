#include "SockEventCallbackUtil.hxx"

#include "LightningServerProcessor.hxx"
#include "session.hxx"

#include "util/logger.hxx"

using namespace Lightning;
//-------------------------------------------------
void SockEventCallbackUtil::onAccepted(evutil_socket_t fd,
            short events,
            void* userData)
{
    if (events & EV_READ)
    {
        LightningServerProcessor* ls = static_cast<LightningServerProcessor*>(userData);
        if (NULL != ls)
        {
            ls->processAccepted(fd);
        }
    }
}

void SockEventCallbackUtil::onSignalHandler(evutil_socket_t, short what, void* arg)
{
    INFO(__FUNCTION__ << " | fire signal : " << what);

    LightningServerProcessor* ls = static_cast<LightningServerProcessor*>(arg);
    if (NULL != ls)
    {
        ls->stop();
    }
}
//-------------------------------------------------

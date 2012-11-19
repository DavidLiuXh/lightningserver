#include "request.hxx"
#include "session.hxx"
#include "userrequest.hxx"

using namespace Lightning;
//---------------------------------------------
StartRequestTask::StartRequestTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session)
        :ProcessQueueItem(type)
        ,mSession(session)
{
}
//---------------------------------------------
ReceivedRequestTask::ReceivedRequestTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session,
                        boost::shared_ptr<UserRequest> request)
:ProcessQueueItem(type)
,mRequest(request)
,mSession(session)
{
}
//---------------------------------------------
ClientClosedTask::ClientClosedTask(ProcessQueueItem::ProcessRequestType type,
            boost::shared_ptr<Session> session)
:ProcessQueueItem(type)
,mSession(session)
{
}
//---------------------------------------------
ErrorTask::ErrorTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session,
                        SessionErrorCode code)
:ProcessQueueItem(type)
,mSession(session)
,mErrorCode(code)
{
}
//---------------------------------------------

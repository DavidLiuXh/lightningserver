#ifndef LIGHTNING_REQUEST_HXX
#define LIGHTNING_REQUEST_HXX

#include "baserequest.hxx"
#include "errorcode.hxx"

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
//----------------------------------------------
namespace Lightning
{
    class Session;
    class UserRequest;

    class StartRequestTask
        :public ProcessQueueItem
    {
        public:
            StartRequestTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session);

        public:
            boost::shared_ptr<Session> getSession() const
            {
                return mSession.lock();
            }

        private:
            boost::weak_ptr<Session> mSession;
    };//class StartRequest

    class ReceivedRequestTask
        :public ProcessQueueItem
    {
        public:
            ReceivedRequestTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session,
                        boost::shared_ptr<UserRequest> request);

            boost::shared_ptr<UserRequest> getRequest() const
            {
                return mRequest;
            }

            boost::shared_ptr<Session> getSession() const
            {
                return mSession.lock();
            }
        private:
            boost::shared_ptr<UserRequest> mRequest;
            boost::weak_ptr<Session> mSession;
    };
    
    class ClientClosedTask 
        :public ProcessQueueItem
    {
        public:
            ClientClosedTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session);

        public:
            boost::shared_ptr<Session> getSession() const
            {
                return mSession.lock();
            }

        private:
            boost::weak_ptr<Session> mSession;
    };//class ClientClosedTask

    class ErrorTask 
        :public ProcessQueueItem
    {
        public:
            ErrorTask(ProcessQueueItem::ProcessRequestType type,
                        boost::shared_ptr<Session> session,
                        SessionErrorCode code);

        public:
            boost::shared_ptr<Session> getSession() const
            {
                return mSession.lock();
            }

            SessionErrorCode getErrorCode() const
            {
                return mErrorCode;
            }
        private:
            boost::weak_ptr<Session> mSession;
            SessionErrorCode mErrorCode;
    };//class ClientClosedTask

}//namespace Lightning
//----------------------------------------------
#endif//#ifndef LIGHTNING_REQUEST_HXX

#ifndef LIGHTNING_RESPONSE_HXX
#define LIGHTNING_RESPONSE_HXX

#include "baserequest.hxx"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
//----------------------------------------------------
namespace Lightning
{
    class UserResponse;
    class Session;
    
    class Response
        :public ProcessQueueItem
    {
        public:
            Response()
                :ProcessQueueItem(PRT_RESPONSE)
            {
            }

            const boost::shared_ptr<UserResponse> getResponse() const
            {
                return mResponse;
            }
            
            void setResponse(boost::shared_ptr<UserResponse> response)
            {
                mResponse = response;
            }
            
            void setSession(boost::shared_ptr<Session> session)
            {
                mSession = session;
            }

            boost::shared_ptr<Session> getSession()
            {
                return mSession.lock();
            }
        private:
            boost::shared_ptr<UserResponse> mResponse;
            boost::weak_ptr<Session> mSession;
    };
}//namespace Lightning
//----------------------------------------------------
#endif//#ifndef LIGHTNING_RESPONSE_HXX

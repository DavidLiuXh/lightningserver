#ifndef LIGHTNING_DATA_HANDLER_HXX
#define LIGHTNING_DATA_HANDLER_HXX

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
//--------------------------------------------------
struct evbuffer;

namespace Lightning
{
    class UserRequest;

    class DataHandler
    {
        public:
            typedef boost::function<void (DataHandler*,
                        boost::shared_ptr<UserRequest>)> RecvRequestFinishedNotifyEvent;
        public:
            virtual ~DataHandler() {}

        public:
            virtual void pushData(evbuffer* buffer) = 0;
            virtual void setUserRequest(boost::shared_ptr<UserRequest> request) = 0;
        
        public:
            RecvRequestFinishedNotifyEvent OnRecvRequestFinished;
    };//class DataHandler
}//namespace Lightning
#endif//#ifndef LIGHTNING_DATA_HANDLER_HXX

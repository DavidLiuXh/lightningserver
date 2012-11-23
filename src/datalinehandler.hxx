#ifndef LIGHTNING_DATA_LINE_HANDLER_HXX
#define LIGHTNING_DATA_LINE_HANDLER_HXX

#include "datahandler.hxx"

#include <event.h>

#include <boost/shared_ptr.hpp>
//--------------------------------------------------
struct evbuffer;

namespace Lightning
{
    class UserRequest;
    class UserRequestWithLine;

    class DataLineHandler
        :public DataHandler
    {
        public:
            DataLineHandler();
            virtual ~DataLineHandler() {}

        public:
            virtual void pushData(evbuffer* buffer);
            virtual void setUserRequest(boost::shared_ptr<UserRequest> request);

        private:
            boost::shared_ptr<evbuffer> mCache;
            boost::shared_ptr<UserRequestWithLine> mUserRequest;
    };//class DataLineHandler
}//namespace Lightning
#endif//#ifndef LIGHTNING_DATA_LINE_HANDLER_HXX

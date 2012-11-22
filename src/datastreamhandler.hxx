#ifndef LIGHTNING_DATA_STREAM_HANDLER_HXX
#define LIGHTNING_DATA_STREAM_HANDLER_HXX

#include "datahandler.hxx"

#include <event.h>

#include <boost/shared_ptr.hpp>
//----------------------------------------------------
namespace Lightning
{
    class UserRequest;
    class UserRequestWithHeader;

    class DataStreamHandler
        :public DataHandler
    {
        private:
            enum ParseStep
            {
                PS_HEADER = 0,
                PS_BODY,
            };
        public:
            DataStreamHandler();

        public:
            virtual void pushData(evbuffer* buffer);
            virtual void setUserRequest(boost::shared_ptr<UserRequest> request);

        private:
            void initEvBuffer();
            bool recvHeader();
            bool recvBody();

        private:
            boost::shared_ptr<evbuffer> mCache;
            size_t mHeaderSize;
            size_t mBodySize;
            ParseStep mStep;
            boost::shared_ptr<UserRequestWithHeader> mUserRequest;
    };//class DataStreamHandler
}//namespace Lightning
#endif//#ifndef LIGHTNING_DATA_STREAM_HANDLER_HXX


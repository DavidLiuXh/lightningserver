#ifndef LIGHTNING_DATA_HANDLER_FACTORY_HXX
#define LIGHTNING_DATA_HANDLER_FACTORY_HXX

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
//----------------------------------------------
namespace Lightning
{
    class DataHandler;
    class UserRequest;

    class DataHandlerFactory
        :public boost::noncopyable
    {
        public:
            enum DataHandlerType
            {
                DHT_STREAM_WITH_HEADER = 0,
            };

        public:
            static boost::shared_ptr<DataHandler> create(DataHandlerType type,
                        boost::shared_ptr<UserRequest>);
        private:
            DataHandlerFactory();

    };//class DataHandlerFactory
}//namespace Lightning
#endif//#ifndef LIGHTNING_DATA_HANDLER_FACTORY_HXX

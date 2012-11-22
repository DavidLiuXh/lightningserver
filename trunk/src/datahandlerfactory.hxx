#ifndef LIGHTNING_DATA_HANDLER_FACTORY_HXX
#define LIGHTNING_DATA_HANDLER_FACTORY_HXX

#include "requesttype.hxx"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
//----------------------------------------------
namespace Lightning
{
    class DataHandler;

    class DataHandlerFactory
        :public boost::noncopyable
    {
        public:
            static boost::shared_ptr<DataHandler> create(RequestType type);
        private:
            DataHandlerFactory();

    };//class DataHandlerFactory
}//namespace Lightning
#endif//#ifndef LIGHTNING_DATA_HANDLER_FACTORY_HXX

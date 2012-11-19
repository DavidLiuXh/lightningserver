#include "datahandlerfactory.hxx"

#include "datastreamhandler.hxx"

#include "util/logger.hxx"

using namespace Lightning;
//-----------------------------------------------------------
boost::shared_ptr<DataHandler> DataHandlerFactory::create(DataHandlerType type,
            boost::shared_ptr<UserRequest> request)
{
    boost::shared_ptr<DataHandler> dataHandler;

    switch (type)
    {
        case DHT_STREAM_WITH_HEADER:
            {
                dataHandler.reset(new DataStreamHandler(request));
            }
            break;
        default:
            {
                ERROR(__FUNCTION__ << " | invaild type of the DataHandler : " << type);
            }
    }

    return dataHandler;

}

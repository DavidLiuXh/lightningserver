#include "datahandlerfactory.hxx"

#include "datastreamhandler.hxx"
#include "datalinehandler.hxx"

#include "util/logger.hxx"

using namespace Lightning;
//-----------------------------------------------------------
boost::shared_ptr<DataHandler> DataHandlerFactory::create(RequestType type)
{
    boost::shared_ptr<DataHandler> dataHandler;

    switch (type)
    {
        case RT_STREAM_WITH_HEADER:
            {
                dataHandler.reset(new DataStreamHandler());
            }
            break;
        case RT_STREAM_WITH_LINE:
            {
                dataHandler.reset(new DataLineHandler());
            }
            break;
        default:
            {
                ERROR(__FUNCTION__ << " | invaild type of the DataHandler : " << type);
            }
    }

    return dataHandler;

}

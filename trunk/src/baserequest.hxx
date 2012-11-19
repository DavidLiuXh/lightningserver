#ifndef LIGHTNING_BASE_REQUEST_HXX 
#define LIGHTNING_BASE_REQUEST_HXX 
//-----------------------------------------------
namespace Lightning
{
    class ProcessQueueItem
    {
        public:
            enum ProcessRequestType
            {
                PRT_NONE = 0,
                PRT_START,
                PRT_REQUEST,
                PRT_RESPONSE,
                PRT_CLISENT_CLOSED,
                PRT_ERROR,
            };

        protected:
            ProcessQueueItem(ProcessRequestType type)
                :mType(type)
            {
            }
            
        public:
            virtual ~ProcessQueueItem() {}
            ProcessRequestType getType() const { return mType; }

        protected:
            ProcessRequestType mType;
    };//calss ProcessItem
}//namespace Lightning
//-----------------------------------------------
#endif//#ifndef LIGHTNING_BASE_REQUEST_HXX 

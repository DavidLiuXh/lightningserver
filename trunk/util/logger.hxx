#ifndef RRRTC_LOGGER_HXX
#define RRRTC_LOGGER_HXX

#include <log4cplus/logger.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
//---------------------------------------------
namespace LUtil 
{
    #define TRACE(msg) LOG4CPLUS_TRACE(LUtil::Logger::instance(), msg) 
    #define INFO(msg) LOG4CPLUS_INFO(LUtil::Logger::instance(), msg) 
    #define DEBUG(msg) LOG4CPLUS_DEBUG(LUtil::Logger::instance(), msg)
    #define WARNING(msg) LOG4CPLUS_WARN(LUtil::Logger::instance(), msg) 
    #define ERROR(msg) LOG4CPLUS_ERROR(LUtil::Logger::instance(), msg)

    class Logger
        :private boost::noncopyable
    {
        public:
            enum LOG_LEVEL
            {
                LL_ALL = 0,
                LL_DEBUG = 10000,
                LL_INFO = 20000,
                LL_WARNING = 30000,
                LL_ERROR = 40000,
            };
        public:
            static void init(LOG_LEVEL level,
                        const char* fileName);
            static log4cplus::Logger& instance()
            {
                return sLogger;
            }
            
        private:
            static log4cplus::Logger sLogger;
    };
}//namesapce rrrtc
#endif //#ifndef RRRTC_LOGGER_HXX

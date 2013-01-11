#ifndef RRRTC_LOGGER_HXX
#define RRRTC_LOGGER_HXX

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
//---------------------------------------------
namespace LUtil 
{
    #define TRACE(msg) \
    do \
    { \
        if (NULL != LUtil::Logger::instance()) \
        { \
LOG4CPLUS_TRACE(*LUtil::Logger::instance(), msg); \
        } \
    }while(0);
    #define INFO(msg) \
     do \
    { \
        if (NULL != LUtil::Logger::instance()) \
        { \
LOG4CPLUS_INFO(*LUtil::Logger::instance(), msg); \
        } \
    }while(0);
    #define DEBUG(msg) \
     do \
    { \
        if (NULL != LUtil::Logger::instance()) \
        { \
LOG4CPLUS_DEBUG(*LUtil::Logger::instance(), msg); \
        } \
    }while(0);
    #define WARNING(msg) \
    do \
    { \
        if (NULL != LUtil::Logger::instance()) \
        { \
LOG4CPLUS_WARN(*LUtil::Logger::instance(), msg); \
        } \
    }while(0);
    #define ERROR(msg) \
    do \
    { \
        if (NULL != LUtil::Logger::instance()) \
        { \
LOG4CPLUS_ERROR(*LUtil::Logger::instance(), msg); \
        } \
    }while(0);

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
                        const char* fileName,
                        bool outputConsole);
            static log4cplus::Logger* instance()
            {
                if (sInit)
                {
                    return &sLogger;
                }
                else
                {
                    return NULL;
                }
            }
            
        private:
            static log4cplus::Logger sLogger;
            static bool sInit;
    };
}//namesapce rrrtc
#endif //#ifndef RRRTC_LOGGER_HXX

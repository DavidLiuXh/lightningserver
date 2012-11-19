#include "logger.hxx"

#include <log4cplus/helpers/pointer.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>

using namespace LUtil;

const char* DEFAULT_LOG_FILE_NAME = "__lutil_logger__.log";
const char* LOGGER_NAME = "__lutil_logger__";
//---------------------------------------------
log4cplus::Logger Logger::sLogger = log4cplus::Logger::getInstance(LOGGER_NAME);

void Logger::init(LOG_LEVEL level,
            const char* fileName)
{
    if (NULL == fileName ||
                '\0' == fileName[0])
    {
        fileName = DEFAULT_LOG_FILE_NAME;
    }

    std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(
                    "[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n"));
    if (NULL != layout.get())
    {
        log4cplus::SharedAppenderPtr fileAppender(new log4cplus::RollingFileAppender
                    (fileName));      
        if (NULL != fileAppender.get())
        {
            fileAppender->setName("file log");
            fileAppender->setLayout(layout);

            sLogger.addAppender(fileAppender);
        }

        log4cplus::SharedAppenderPtr consoleAppender(new log4cplus::ConsoleAppender());      
        if (NULL != consoleAppender.get())
        {
            consoleAppender->setName("console log");
            std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(
                            //"[%p] [%D{%m/%d/%y %H:%M:%S:%s}] [%t] [%l] - %m %n"));
                "[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n"));
            if (NULL != layout.get())
            {
                consoleAppender->setLayout(layout);
            }

            sLogger.addAppender(consoleAppender);
        }

        sLogger.setLogLevel(log4cplus::LogLevel(level));
    }
}
//---------------------------------------------

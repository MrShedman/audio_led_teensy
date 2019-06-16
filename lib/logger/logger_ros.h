#pragma once

#include "ros.h"
#include "logger.h"

namespace ros
{

template<uint32_t BUFFER_SIZE = 128>
class Logger_ : public LoggerBase
{
public:

    explicit Logger_(NodeHandle& nh)
    :
    nh_(nh)
    {
        memset(log_buffer, 0, BUFFER_SIZE);
        setLogger(this);
    }

    ~Logger_()
    {
        setLogger(nullptr);
    }

    void print(Level level, const char* file, int line, const char* fmt, ...) override
    {
        memset(log_buffer, 0, BUFFER_SIZE);
        va_list args;
        va_start(args, fmt);

        const int32_t result = vsnprintf(log_buffer, BUFFER_SIZE, fmt, args);

        va_end(args);

        if (result > 0 && result < (int32_t)BUFFER_SIZE)
        {
            // success
            switch (level)
            {
                case Level::Debug: return nh_.logdebug(log_buffer);
                case Level::Info:  return nh_.loginfo(log_buffer);
                case Level::Warn:  return nh_.logwarn(log_buffer);
                case Level::Error: return nh_.logerror(log_buffer);
                case Level::Fatal: return nh_.logfatal(log_buffer);
                default:    return nh_.logerror(log_error_msg);
            }
        }
        else
        {
            return nh_.logerror(log_error_msg);
        }
    }

private:

    char log_buffer[BUFFER_SIZE];

    const char* log_error_msg = "Error writing log!";

    NodeHandle nh_;
};

typedef Logger_<> Logger;

}
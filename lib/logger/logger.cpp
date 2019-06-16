#include "logger.h"

#include <cstdint>

const uint8_t logloc_array_size = 64;
LogLocation* logloc_array[logloc_array_size];
uint8_t logloc_index = 0;
LoggerBase* logger_ptr = nullptr;

void setLogger(LoggerBase* log_ptr)
{
    logger_ptr = log_ptr;
}

void registerLogLocation(LogLocation* loc)
{
    logloc_array[logloc_index] = loc;
    logloc_index++;
}

void initializeLogLocation(LogLocation* loc, Level level)
{
    if (loc->initialized_)
    {
        return;
    }

    registerLogLocation(loc);

    loc->level_ = level;
    loc->initialized_ = true;
}

void setLogLocationLevel(LogLocation* loc, Level level)
{
    loc->level_ = level;
}

const char* getLevelString(Level level)
{
    switch (level)
    {
        case Level::Debug: return "DEBUG";
        case Level::Info:  return "INFO";
        case Level::Warn:  return "WARN";
        case Level::Error: return "ERROR";
        case Level::Fatal: return "FATAL";
        default: return "UNKNO";
    }
}
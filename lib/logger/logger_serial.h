#pragma once

#include <Arduino.h>

#include "logger.h"
#include "Stream.h"

class LoggerSerial : public LoggerBase
{
public:

    LoggerSerial(Stream* stream_ptr = &Serial)
    :
    stream_ptr(stream_ptr)
    {
        setLogger(this);
    }

    void print(Level level, const char* file, int line, const char* fmt, ...) override
    {
        const float now = (float)millis() / 1000.0;
        Serial.printf("[%s] [%.3f]: ", getLevelString(level), now);

        va_list ap;
        va_start(ap, fmt);

        vdprintf((int)stream_ptr, fmt, ap);

        Serial.println();
    }

private:

    Stream* stream_ptr;
};

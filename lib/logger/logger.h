#pragma once

enum class Level
{
    Debug,
    Info,
    Warn,
    Error,
    Fatal,

    Count
};

struct LogLocation
{
    bool initialized_;
    Level level_;
};

class LoggerBase
{
public:
    virtual ~LoggerBase(){}
    virtual void print(Level level, const char* file, int line, const char* fmt, ...) = 0;
};

extern LoggerBase* logger_ptr;

void setLogger(LoggerBase* log_ptr);

void initializeLogLocation(LogLocation* loc, Level level);

void setLogLocationLevel(LogLocation* loc, Level level);

const char* getLevelString(Level level);

#define COND_LIKELY(x)       __builtin_expect((x),1)
#define COND_UNLIKELY(x)     __builtin_expect((x),0)

// These allow you to compile-out everything below a certain severity level if necessary
#define LOG_SEVERITY_DEBUG 0
#define LOG_SEVERITY_INFO  1
#define LOG_SEVERITY_WARN  2
#define LOG_SEVERITY_ERROR 3
#define LOG_SEVERITY_FATAL 4
#define LOG_SEVERITY_NONE  5

/**
 * \def LOG_MIN_SEVERITY
 *
 * Define LOG_MIN_SEVERITY=LOG_SEVERITY_[DEBUG|INFO|WARN|ERROR|FATAL] in your build options to compile out anything below that severity
 */
#ifndef LOG_MIN_SEVERITY
#define LOG_MIN_SEVERITY LOG_SEVERITY_DEBUG
#endif

#define LOG_DEFINE_LOCATION(cond, level) \
  static LogLocation loc = {false, Level::Count}; /* Initialized at compile-time */ \
  if (COND_UNLIKELY(!loc.initialized_)) \
  { \
    initializeLogLocation(&loc, level); \
  } \
  if (COND_UNLIKELY(loc.level_ != level)) \
  { \
    setLogLocationLevel(&loc, level); \
  } \
  bool enabled = cond;


#define LOG_PRINT_AT_LOCATION(...) \
    if (logger_ptr) \
        logger_ptr->print(loc.level_, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_COND(cond, level, ...) \
  do \
  { \
    LOG_DEFINE_LOCATION(cond, level); \
    \
    if (COND_UNLIKELY(enabled)) \
    { \
      LOG_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)

#define LOG_ONCE(level, ...) \
  do \
  { \
    LOG_DEFINE_LOCATION(true, level); \
    static bool hit = false; \
    if (COND_UNLIKELY(enabled) && COND_UNLIKELY(!hit)) \
    { \
      hit = true; \
      LOG_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)

#define LOG_THROTTLE(rate, level, ...) \
  do \
  { \
    LOG_DEFINE_LOCATION(true, level); \
    static double last_hit = 0.0; \
    double now = (double)millis() / 1000.0; \
    if (COND_UNLIKELY(enabled) && COND_UNLIKELY(last_hit + (1.0/rate) <= now)) \
    { \
      last_hit = now; \
      LOG_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)

#define LOG(level, ...) LOG_COND(true, level, __VA_ARGS__)

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_DEBUG)
#define LOG_DEBUG(...)
#define LOG_DEBUG_COND(cond, ...)
#define LOG_DEBUG_ONCE(...))
#define LOG_DEBUG_THROTTLE(rate, ...)
#else
#define LOG_DEBUG(...) LOG(Level::Debug, __VA_ARGS__)
#define LOG_DEBUG_COND(cond, ...) LOG_COND(cond, Level::Debug, __VA_ARGS__)
#define LOG_DEBUG_ONCE(...) LOG_ONCE(Level::Debug, __VA_ARGS__)
#define LOG_DEBUG_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Debug, __VA_ARGS__)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_INFO)
#define LOG_INFO(...)
#define LOG_INFO_COND(cond, ...)
#define LOG_INFO_ONCE(...)
#define LOG_INFO_THROTTLE(rate, ...)
#else
#define LOG_INFO(...) LOG(Level::Info, __VA_ARGS__)
#define LOG_INFO_COND(cond, ...) LOG_COND(cond, Level::Info, __VA_ARGS__)
#define LOG_INFO_ONCE(...) LOG_ONCE(Level::Info, __VA_ARGS__)
#define LOG_INFO_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Info, __VA_ARGS__)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_WARN)
#define LOG_WARN(...)
#define LOG_WARN_COND(cond, ...)
#define LOG_WARN_ONCE(...)
#define LOG_WARN_THROTTLE(rate, ...)
#else
#define LOG_WARN(...) LOG(Level::Warn, __VA_ARGS__)
#define LOG_WARN_COND(cond, ...) LOG_COND(cond, Level::Warn, __VA_ARGS__)
#define LOG_WARN_ONCE(...) LOG_ONCE(Level::Warn, __VA_ARGS__)
#define LOG_WARN_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Warn, __VA_ARGS__)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_ERROR)
#define LOG_ERROR(...)
#define LOG_ERROR_COND(cond, ...)
#define LOG_ERROR_ONCE(...)
#define LOG_ERROR_THROTTLE(rate, ...)
#else
#define LOG_ERROR(...) LOG(Level::Error, __VA_ARGS__)
#define LOG_ERROR_COND(cond, ...) LOG_COND(cond, Level::Error, __VA_ARGS__)
#define LOG_ERROR_ONCE(...) LOG_ONCE(Level::Error, __VA_ARGS__)
#define LOG_ERROR_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Error, __VA_ARGS__)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_FATAL)
#define LOG_FATAL(...)
#define LOG_FATAL_COND(cond, ...)
#define LOG_FATAL_ONCE(...)
#define LOG_FATAL_THROTTLE(rate, ...)
#else
#define LOG_FATAL(...) LOG(Level::Fatal, __VA_ARGS__)
#define LOG_FATAL_COND(cond, ...) LOG_COND(cond, Level::Fatal, __VA_ARGS__)
#define LOG_FATAL_ONCE(...) LOG_ONCE(Level::Fatal, __VA_ARGS__)
#define LOG_FATAL_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Fatal, __VA_ARGS__)
#endif
#include <chrono>
#include <ctime>
#include <mutex>

#include "DgeX/Common/Log.h"

DGEX_BEGIN

static const char* GetLogLevelString(LogLevel level);
static const char* GetTimestamp();

LogLevel Log::_logLevel = LogLevel::Debug;
FILE* Log::_logFile = nullptr;
bool Log::_console = true;


/**
 * @brief This class ensures that the log is closed when the program exits.
 */
class LogGuard
{
public:
    ~LogGuard()
    {
        DGEX_LOG_FINE("Log closed");
        Log::_Close();
    }
};


static LogGuard logGuard;
static std::mutex logMutex;


void Log::Init(LogLevel level, const char* logFile, bool console)
{
    _logLevel = level;
    _console = console;
    if (logFile != nullptr)
    {
        fopen_s(&_logFile, logFile, "w");
        if (_logFile == nullptr)
        {
            // Error opening the log file, silently ignore.
        }
    }
}


void Log::Fine(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Fine, format, args);
    va_end(args);
}


void Log::Debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Debug, format, args);
    va_end(args);
}


void Log::Info(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Info, format, args);
    va_end(args);
}


void Log::Warning(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Warning, format, args);
    va_end(args);
}


void Log::Error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Error, format, args);
    va_end(args);
}


void Log::Critical(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Critical, format, args);
    va_end(args);
}


void Log::_Log(LogLevel level, const char* format, va_list args)
{
    if (level < _logLevel)
    {
        return;
    }

    std::lock_guard lock(logMutex);

    if (_console)
    {
        fprintf(stderr, "[%s] [%s] ", GetTimestamp(), GetLogLevelString(level));
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
    }

    if (_logFile != nullptr)
    {
        fprintf(_logFile, "[%s] [%s] ", GetTimestamp(), GetLogLevelString(level));
        vfprintf(_logFile, format, args);
        fprintf(_logFile, "\n");

        // Flush the log file to ensure that the log is written to disk
        fflush(_logFile);
    }
}


void Log::_Close()
{
    if (_logFile != nullptr)
    {
        fclose(_logFile);
        _logFile = nullptr;
    }
}


static const char* GetLogLevelString(LogLevel level)
{
    switch (level)
    {
        using enum LogLevel;
    case All:
        return "ALL";
    case Fine:
        return "FINE";
    case Debug:
        return "DEBUG";
    case Info:
        return "INFO";
    case Warning:
        return "WARNING";
    case Error:
        return "ERROR";
    case Critical:
        return "CRITICAL";
    case Disabled:
        return "DISABLED";
    }
    return "UNKNOWN";
}


static const char* GetTimestamp()
{
    static char timestampBuffer[128];

    const auto now = std::chrono::system_clock::now();
    const std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm nowTm;
    localtime_s(&nowTm, &now_c);
    strftime(timestampBuffer, sizeof(timestampBuffer), "%Y-%m-%d %H:%M:%S", &nowTm);
    return timestampBuffer;
}


DGEX_END

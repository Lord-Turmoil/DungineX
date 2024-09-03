#include <chrono>
#include <ctime>
#include <mutex>

#include "DgeX/Common/Log.h"

DGEX_BEGIN

static const wchar_t* GetLogLevelString(LogLevel level);
static const wchar_t* GetTimestamp();

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
        DGEX_LOG_FINE(L"Log closed");
        Log::_Close();
    }
};


static LogGuard logGuard;
static std::mutex logMutex;


void Log::Init(LogLevel level, const wchar_t* logFile, bool console)
{
    _logLevel = level;
    _console = console;
    if (logFile != nullptr)
    {
        if (_logFile != nullptr)
        {
            fclose(_logFile);
        }
        _wfopen_s(&_logFile, logFile, L"a");
    }

    DGEX_LOG_INFO(L"Log ready at level '%s'", GetLogLevelString(level));
}


void Log::Fine(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Fine, format, args);
    va_end(args);
}


void Log::Debug(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Debug, format, args);
    va_end(args);
}


void Log::Info(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Info, format, args);
    va_end(args);
}


void Log::Warning(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Warning, format, args);
    va_end(args);
}


void Log::Error(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Error, format, args);
    va_end(args);
}


void Log::Critical(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    _Log(LogLevel::Critical, format, args);
    va_end(args);
}


void Log::_Log(LogLevel level, const wchar_t* format, va_list args)
{
    if (level < _logLevel)
    {
        return;
    }

    std::lock_guard lock(logMutex);

    if (_console)
    {
        fwprintf(stderr, L"[%s] [%s] ", GetTimestamp(), GetLogLevelString(level));
        vfwprintf(stderr, format, args);
        fwprintf(stderr, L"\n");
    }

    if (_logFile != nullptr)
    {
        fwprintf(_logFile, L"[%s] [%s] ", GetTimestamp(), GetLogLevelString(level));
        vfwprintf(_logFile, format, args);
        fwprintf(_logFile, L"\n");

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


static const wchar_t* GetLogLevelString(LogLevel level)
{
    switch (level)
    {
        using enum LogLevel;
    case All:
        return L"ALL";
    case Fine:
        return L"FINE";
    case Debug:
        return L"DEBUG";
    case Info:
        return L"INFO";
    case Warning:
        return L"WARNING";
    case Error:
        return L"ERROR";
    case Critical:
        return L"CRITICAL";
    case Disabled:
        return L"DISABLED";
    }
    return L"UNKNOWN";
}


static const wchar_t* GetTimestamp()
{
    static wchar_t timestampBuffer[128];

    const auto now = std::chrono::system_clock::now();
    const std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm nowTm;
    localtime_s(&nowTm, &now_c);
    wcsftime(timestampBuffer, sizeof(timestampBuffer), L"%Y-%m-%d %H:%M:%S", &nowTm);
    return timestampBuffer;
}


DGEX_END

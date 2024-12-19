#pragma once

#include "DgeX/Common/Base.h"

#include <spdlog/spdlog.h>

#include <filesystem>

DGEX_BEGIN

/**
 * @brief Log level. Unfortunately, the all-caps ERROR is already defined
 * somewhere in Windows headers, so we use PascalCase instead.
 */
enum class LogLevel : unsigned char
{
    All,
    Fine,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
    Disabled
};

const char DEFAULT_LOG_FILE[] = "DgeX.log";

/**
 * @brief The class to support debug logging.
 *        It uses lock to ensure thread-safety.
 */
class Log
{
    friend class LogGuard;

public:
    static void Init(LogLevel level, bool console, const char* filename);

    static void Init(LogLevel level, bool console)
    {
        Init(level, console, DEFAULT_LOG_FILE);
    }

    static void Init(LogLevel level)
    {
        Init(level, true, DEFAULT_LOG_FILE);
    }

    static void Init()
    {
        Init(LogLevel::All);
    }

    static Ref<spdlog::logger>& GetCoreLogger()
    {
        return _sCoreLogger;
    }

    static Ref<spdlog::logger>& GetClientLogger()
    {
        return _sClientLogger;
    }

private:
    static Ref<spdlog::logger> _sCoreLogger;
    static Ref<spdlog::logger> _sClientLogger;
};

#ifdef DGEX_ENABLE_LOGGING

#ifdef _DGEX_CORE_
#define DGEX_CORE_TRACE(...)    DgeX::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DGEX_CORE_DEBUG(...)    DgeX::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define DGEX_CORE_INFO(...)     DgeX::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DGEX_CORE_WARN(...)     DgeX::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DGEX_CORE_ERROR(...)    DgeX::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DGEX_CORE_CRITICAL(...) DgeX::Log::GetCoreLogger()->critical(__VA_ARGS__)
#endif

#define DGEX_LOG_TRACE(...)    DgeX::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DGEX_LOG_DEBUG(...)    DgeX::Log::GetClientLogger()->debug(__VA_ARGS__)
#define DGEX_LOG_INFO(...)     DgeX::Log::GetClientLogger()->info(__VA_ARGS__)
#define DGEX_LOG_WARN(...)     DgeX::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DGEX_LOG_ERROR(...)    DgeX::Log::GetClientLogger()->error(__VA_ARGS__)
#define DGEX_LOG_CRITICAL(...) DgeX::Log::GetClientLogger()->critical(__VA_ARGS__)

#else

#ifdef __DGEX_CORE__
#define DGEX_CORE_TRACE(...)
#define DGEX_CORE_DEBUG(...)
#define DGEX_CORE_INFO(...)
#define DGEX_CORE_WARN(...)
#define DGEX_CORE_ERROR(...)
#define DGEX_CORE_CRITICAL(...)
#endif

#define DGEX_LOG_TRACE(...)
#define DGEX_LOG_DEBUG(...)
#define DGEX_LOG_INFO(...)
#define DGEX_LOG_WARN(...)
#define DGEX_LOG_ERROR(...)
#define DGEX_LOG_CRITICAL(...)

#endif

#ifdef _DGEX_CORE_
#define DGEX_TIME_LOG DGEX_CORE_INFO
#else
#define DGEX_TIME_LOG DGEX_LOG_INFO
#endif

class TimeLog
{
public:
    TimeLog(std::string message, std::string file, int startLine)
        : _message(std::move(message)), _start(std::chrono::steady_clock::now()), _file(std::move(file)),
          _startLine(startLine), _endLine(0)
    {
        DGEX_TIME_LOG(">>>>> [{}] {}:{}", _message, _file, _startLine);
    }

    void End(int endLine)
    {
        _endLine = endLine;
    }

    ~TimeLog()
    {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count();
        DGEX_TIME_LOG("<<<<< [{}] {}:{} in {:.4} s", _message, _file, _endLine, static_cast<double>(duration) / 1000.0);
    }

private:
    std::string _message;
    std::chrono::time_point<std::chrono::steady_clock> _start;
    std::string _file;
    int _startLine;
    int _endLine;
};

#ifdef DGEX_ENABLE_TIMING

#define _DGEX_TIME(message, file, line) DgeX::TimeLog __time_log(std::string(message), file, line)

#define DGEX_TIME(message) _DGEX_TIME(message, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define DGEX_TIME_BEGIN(message)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
    DGEX_TIME(message)
#define DGEX_TIME_END()                                                                                                \
    __time_log.End(__LINE__);                                                                                          \
    }                                                                                                                  \
    while (0)

#else

#define _DGEX_TIME_BEGIN(message, file, line) DGEX_TIME_LOG(">>>>> [{}] {}:{}", message, file, line)
#define _DGEX_TIME_END(file, line)            DGEX_TIME_LOG("<<<<< {}:{}", file, line)

#define DGEX_TIME(message) DGEX_TIME_LOG(message)
#define DGEX_TIME_BEGIN(message)                                                                                       \
    _DGEX_TIME_BEGIN(message, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define DGEX_TIME_END() _DGEX_TIME_END(std::filesystem::path(__FILE__).filename().string(), __LINE__)

#endif

DGEX_END

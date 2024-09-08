#pragma once

#include <spdlog/spdlog.h>

#include "DgeX/Common/Base.h"
#include "DgeX/Common/Macros.h"

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

DGEX_END

#pragma once

#include <cstdarg>
#include <cstdio>

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


/**
 * @brief The class to support debug logging.
 *        It uses lock to ensure thread-safety.
 */
class Log
{
    friend class LogGuard;

public:
    static void Init(LogLevel level, const wchar_t* logFile, bool console);
    static void Init(LogLevel level, const wchar_t* logFile) { Init(level, logFile, false); }
    static void Init(LogLevel level) { Init(level, nullptr, true); }


    static void Fine(const wchar_t* format, ...);
    static void Debug(const wchar_t* format, ...);
    static void Info(const wchar_t* format, ...);
    static void Warning(const wchar_t* format, ...);
    static void Error(const wchar_t* format, ...);
    static void Critical(const wchar_t* format, ...);

private:
    static void _Log(LogLevel level, const wchar_t* format, va_list args);
    static void _Close();

    static LogLevel _logLevel;
    static FILE* _logFile;
    static bool _console;
};


#ifdef DGEX_ENABLE_TRACE
#define DGEX_LOG_FINE(...) DgeX::Log::Fine(__VA_ARGS__)
#define DGEX_LOG_DEBUG(...) DgeX::Log::Debug(__VA_ARGS__)
#define DGEX_LOG_INFO(...) DgeX::Log::Info(__VA_ARGS__)
#define DGEX_LOG_WARNING(...) DgeX::Log::Warning(__VA_ARGS__)
#define DGEX_LOG_ERROR(...) DgeX::Log::Error(__VA_ARGS__)
#define DGEX_LOG_CRITICAL(...) DgeX::Log::Critical(__VA_ARGS__)
#else
#define DGEX_LOG_FINE(...) DgeX::Log::Fine(__VA_ARGS__)
#define DGEX_LOG_DEBUG(...) DgeX::Log::Debug(__VA_ARGS__)
#define DGEX_LOG_INFO(...) DgeX::Log::Info(__VA_ARGS__)
#define DGEX_LOG_WARNING(...) DgeX::Log::Warning(__VA_ARGS__)
#define DGEX_LOG_ERROR(...) DgeX::Log::Error(__VA_ARGS__)
#define DGEX_LOG_CRITICAL(...) DgeX::Log::Critical(__VA_ARGS__)
#endif

DGEX_END

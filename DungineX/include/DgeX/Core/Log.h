#pragma once

#include "DgeX/Core/Base.h"
#include "DgeX/Core/Logger.h"

#include <spdlog/spdlog.h>
#include <tinyxml2.h>

#include <filesystem>

DGEX_BEGIN

namespace Log
{
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
    Disabled,
    Invalid
};

LogLevel LogLevelFromString(std::string level);
LogLevel LogLevelFromString(std::string level, LogLevel defaultLevel);

struct LoggerSink
{
    /**
     * @brief The output file path.
     * @note Use "stdout" for console output and "stderr" for error output.
     */
    std::string File;

    /**
     * @brief Log message pattern.
     * @see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#customizing-format-using-set_pattern
     */
    std::string Pattern;

    LoggerSink() = default;
    LoggerSink(std::string file, std::string pattern);
    LoggerSink(std::string file);
};

struct LoggerSpecification
{
    LogLevel Level;
    std::string Name;
    std::vector<LoggerSink> Sinks;

    LoggerSpecification() = default;
    LoggerSpecification(std::string name, LogLevel level, std::initializer_list<LoggerSink> sinks);
    LoggerSpecification(std::string name, LogLevel level, const std::vector<LoggerSink>& sinks);
    LoggerSpecification(std::string name, LogLevel level, const LoggerSink& sink);
};

void Init(const char* filename);
void Init(const tinyxml2::XMLElement& config);
void Init(const std::vector<LoggerSpecification>& specifications);

Ref<Logger> GetCoreLogger();
Ref<Logger> GetClientLogger();
Ref<Logger> GetLogger(const std::string& name);

} // namespace Log

#ifdef DGEX_ENABLE_LOGGING

#ifdef _DGEX_CORE_
#define _DGEX_LOGGER DGEX Log::GetCoreLogger()
#else
#define _DGEX_LOGGER DGEX Log::GetClientLogger()
#endif
#define _DGEX_CUSTOM_LOGGER(NAME) DGEX Log::GetLogger(NAME)

#define DGEX_LOG_TRACE(...)    _DGEX_LOGGER->Trace(__VA_ARGS__)
#define DGEX_LOG_DEBUG(...)    _DGEX_LOGGER->Debug(__VA_ARGS__)
#define DGEX_LOG_INFO(...)     _DGEX_LOGGER->Info(__VA_ARGS__)
#define DGEX_LOG_WARN(...)     _DGEX_LOGGER->Warn(__VA_ARGS__)
#define DGEX_LOG_ERROR(...)    _DGEX_LOGGER->Error(__VA_ARGS__)
#define DGEX_LOG_CRITICAL(...) _DGEX_LOGGER->Critical(__VA_ARGS__)

#define DGEX_NLOG_TRACE(NAME, ...)    _DGEX_CUSTOM_LOGGER(NAME)->Trace(__VA_ARGS__)
#define DGEX_NLOG_DEBUG(NAME, ...)    _DGEX_CUSTOM_LOGGER(NAME)->Debug(__VA_ARGS__)
#define DGEX_NLOG_INFO(NAME, ...)     _DGEX_CUSTOM_LOGGER(NAME)->Info(__VA_ARGS__)
#define DGEX_NLOG_WARN(NAME, ...)     _DGEX_CUSTOM_LOGGER(NAME)->Warn(__VA_ARGS__)
#define DGEX_NLOG_ERROR(NAME, ...)    _DGEX_CUSTOM_LOGGER(NAME)->Error(__VA_ARGS__)
#define DGEX_NLOG_CRITICAL(NAME, ...) _DGEX_CUSTOM_LOGGER(NAME)->Critical(__VA_ARGS__)

#else

#define DGEX_LOG_TRACE(...)
#define DGEX_LOG_DEBUG(...)
#define DGEX_LOG_INFO(...)
#define DGEX_LOG_WARN(...)
#define DGEX_LOG_ERROR(...)
#define DGEX_LOG_CRITICAL(...)

#endif

#ifdef DGEX_ENABLE_TIMING

#define DGEX_TIME_LOG DGEX_LOG_INFO

namespace Log
{

class _TimeLog
{
public:
    _TimeLog(std::string message, std::string file, int startLine)
        : _message(std::move(message)), _start(std::chrono::steady_clock::now()), _file(std::move(file)),
          _startLine(startLine), _endLine(0)
    {
        DGEX_TIME_LOG(">>>>> [{}] {}:{}", _message, _file, _startLine);
    }

    void End(int endLine)
    {
        _endLine = endLine;
    }

    ~_TimeLog()
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

} // namespace Log

#define _DGEX_TIME(message, file, line) DGEX Log::_TimeLog __time_log(std::string(message), file, line)

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

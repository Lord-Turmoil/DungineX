/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Log.h                                     *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 1, 2025                              *
 *                                                                            *
 *                    Last Update : June 19, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Logging support.                                                           *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Utils/Types.h"

#include <spdlog/spdlog.h>

#include <string>
#include <unordered_map>
#include <vector>

DGEX_BEGIN

/**
 * @brief Log levels are corresponding to that of spdlog.
 */
enum class LogLevel : unsigned char
{
    All,      // all logs
    Fine,     // logs fine and above, equals to all
    Debug,    // logs debug and above
    Info,     // logs info and above
    Warn,     // logs warn(ing) and above
    Error,    // logs error and above
    Critical, // logs critical and above
    Disabled, // nothing will be logged
};

/**
 * @brief Sink configuration for a logger.
 *
 * Different sinks of a logger can have different log levels and paths.
 */
struct LoggerSinkSpecification
{
    // Log file path, can be one of the following:
    //
    // - stdout
    // - stderr
    // - file path
    std::string Path;

    // Log message pattern, See spdlog's documentation for details:
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#customizing-format-using-set_pattern
    std::string Pattern;

    /**
     * @brief Create a logger sink specification with a file path.
     *
     * DungineX will create default pattern for the log file.
     *
     * @param path Log file path.
     */
    DGEX_API LoggerSinkSpecification(std::string path);

    /**
     * @brief Create a logger sink specification with a file path and pattern.
     *
     * @param path Log file path.
     * @param pattern Log info pattern.
     */
    DGEX_API LoggerSinkSpecification(std::string path, std::string pattern);
};

/**
 * @brief Specification for a logger, with name and sinks.
 */
struct LoggerSpecification
{
    // Name of the logger.
    std::string Name;

    // Log level.
    LogLevel Level;

    // Where to output logs.
    std::vector<LoggerSinkSpecification> Sinks;

    /**
     * @brief Create a logger specification.
     *
     * @param name Name of the logger.
     * @param level Log level.
     * @param sinks Multiple sinks.
     */
    DGEX_API LoggerSpecification(std::string name, LogLevel level,
                                 std::initializer_list<LoggerSinkSpecification> sinks);
};

/**
 * @brief The logger.
 *
 * I prefer PascalCase for methods, so I wraps the original functions from spdlog.
 */
class Logger
{
public:
    explicit Logger(const LoggerSpecification& specification);

    Logger(const Logger& other) = delete;
    Logger(Logger&& other) noexcept = delete;
    Logger& operator=(const Logger& other) = delete;
    Logger& operator=(Logger&& other) noexcept = delete;

    ~Logger();

public:
    DGEX_API const std::string& GetName() const;

public:
    template <typename T> DGEX_API void Trace(const T& msg)
    {
        _impl->trace(msg);
    }

    template <typename... Args> DGEX_API void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _impl->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename T> DGEX_API void Debug(const T& msg)
    {
        _impl->debug(msg);
    }

    template <typename... Args> DGEX_API void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _impl->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename T> DGEX_API void Info(const T& msg)
    {
        _impl->info(msg);
    }

    template <typename... Args> DGEX_API void Info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _impl->info(fmt, std::forward<Args>(args)...);
    }

    template <typename T> DGEX_API void Warn(const T& msg)
    {
        _impl->warn(msg);
    }

    template <typename... Args> DGEX_API void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _impl->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename T> DGEX_API void Error(const T& msg)
    {
        _impl->error(msg);
    }

    template <typename... Args> DGEX_API void Error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _impl->error(fmt, std::forward<Args>(args)...);
    }

    template <typename T> DGEX_API void Critical(const T& msg)
    {
        _impl->critical(msg);
    }

    template <typename... Args> DGEX_API void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        _impl->critical(fmt, std::forward<Args>(args)...);
    }

private:
    std::string _name;
    Ref<spdlog::logger> _impl;
};

/**
 * @brief Log utility.
 */
class Log
{
public:
    /**
     * @brief Get a logger with a name.
     *
     * If the logger is not registered, a default logger that logs to stderr
     * will be created and returned.
     *
     * @param name Name of the logger.
     * @return A logger with the specified name.
     */
    DGEX_API static Ref<Logger> GetLogger(const std::string& name);

    /**
     * @brief Register a configured logger.
     *
     * @tparam Args Auto-deduct arguments types of Logger's ctor.
     * @param specification Logger specification.
     * @return A registered logger.
     */
    DGEX_API static Ref<Logger> RegisterLogger(const LoggerSpecification& specification);

    /**
     * @brief Initialize built-in loggers.
     */
    static void Init();

private:
    Log() = default;

    static std::unordered_map<std::string, Ref<Logger>> _sLoggers;
};

DGEX_END

// ============================================================================
// Logger Macros
// ----------------------------------------------------------------------------

#define _DGEX_CORE_LOGGER_NAME DUNGINEX_SHORT

#define _DGEX_LOGGER(NAME) DGEX Log::GetLogger(NAME)
#define _DGEX_CORE_LOGGER  _DGEX_LOGGER(_DGEX_CORE_LOGGER_NAME)

#define DGEX_CORE_TRACE(...)    _DGEX_CORE_LOGGER->Trace(__VA_ARGS__)
#define DGEX_CORE_DEBUG(...)    _DGEX_CORE_LOGGER->Debug(__VA_ARGS__)
#define DGEX_CORE_INFO(...)     _DGEX_CORE_LOGGER->Info(__VA_ARGS__)
#define DGEX_CORE_WARN(...)     _DGEX_CORE_LOGGER->Warn(__VA_ARGS__)
#define DGEX_CORE_ERROR(...)    _DGEX_CORE_LOGGER->Error(__VA_ARGS__)
#define DGEX_CORE_CRITICAL(...) _DGEX_CORE_LOGGER->Critical(__VA_ARGS__)

#define DGEX_LOG_TRACE(NAME, ...)    _DGEX_LOGGER(NAME)->Trace(__VA_ARGS__)
#define DGEX_LOG_DEBUG(NAME, ...)    _DGEX_LOGGER(NAME)->Debug(__VA_ARGS__)
#define DGEX_LOG_INFO(NAME, ...)     _DGEX_LOGGER(NAME)->Info(__VA_ARGS__)
#define DGEX_LOG_WARN(NAME, ...)     _DGEX_LOGGER(NAME)->Warn(__VA_ARGS__)
#define DGEX_LOG_ERROR(NAME, ...)    _DGEX_LOGGER(NAME)->Error(__VA_ARGS__)
#define DGEX_LOG_CRITICAL(NAME, ...) _DGEX_LOGGER(NAME)->Critical(__VA_ARGS__)

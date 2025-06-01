/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Log.cpp                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 1, 2025                              *
 *                                                                            *
 *                    Last Update : June 1, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Logging support.                                                           *
 ******************************************************************************/

#include "DgeX/Utils/Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

DGEX_BEGIN

static constexpr char DEFAULT_COLOR_PATTERN[] = "[%Y-%m-%d %H:%M:%S] %^%8l%$ [%6n]: %v";
static constexpr char DEFAULT_PATTERN[] = "[%Y-%m-%d %H:%M:%S] %l [%n]: %v";

static spdlog::level::level_enum LogLevelToSpdLogLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::All:
    case LogLevel::Fine:
        return spdlog::level::trace;
    case LogLevel::Debug:
        return spdlog::level::debug;
    case LogLevel::Info:
        return spdlog::level::info;
    case LogLevel::Warn:
        return spdlog::level::warn;
    case LogLevel::Error:
        return spdlog::level::err;
    case LogLevel::Critical:
        return spdlog::level::critical;
    case LogLevel::Disabled:
        return spdlog::level::off;
    }
    return spdlog::level::off;
}

#ifdef DGEX_DEBUG
#define DEFAULT_LOG_LEVEL LogLevel::Debug
#elif !defined(DGEX_PUBLISH)
#define DEFAULT_LOG_LEVEL LogLevel::Info
#else
#define DEFAULT_LOG_LEVEL LogLevel::Warn
#endif

std::unordered_map<std::string, Ref<Logger>> Log::_sLoggers;

LoggerSinkSpecification::LoggerSinkSpecification(std::string path) : Path(std::move(path))
{
    if ((Path == "stdout") || (Path == "stderr"))
    {
        Pattern = DEFAULT_COLOR_PATTERN;
    }
    else
    {
        Pattern = DEFAULT_PATTERN;
    }
}

LoggerSinkSpecification::LoggerSinkSpecification(std::string path, std::string pattern)
    : Path(std::move(path)), Pattern(std::move(pattern))
{
}

LoggerSpecification::LoggerSpecification(std::string name, LogLevel level,
                                         std::initializer_list<LoggerSinkSpecification> sinks)
    : Name(std::move(name)), Level(level), Sinks(sinks)
{
}

Logger::Logger(const LoggerSpecification& specification) : _name(specification.Name)
{
    std::vector<spdlog::sink_ptr> sinks;

    for (const auto& sink : specification.Sinks)
    {
        spdlog::sink_ptr sinkImpl;

        if (sink.Path == "stdout")
        {
            sinkImpl = CreateRef<spdlog::sinks::stdout_color_sink_mt>();
        }
        else if (sink.Path == "stderr")
        {
            sinkImpl = CreateRef<spdlog::sinks::stderr_color_sink_mt>();
        }
        else
        {
            sinkImpl = CreateRef<spdlog::sinks::basic_file_sink_mt>(sink.Path, true);
        }
        sinkImpl->set_pattern(sink.Pattern);

        sinks.push_back(sinkImpl);
    }

    _impl = CreateRef<spdlog::logger>(specification.Name, begin(sinks), end(sinks));
    _impl->set_level(LogLevelToSpdLogLevel(specification.Level));

    register_logger(_impl);
}

Logger::~Logger()
{
    // FIXME:
    // This seems unnecessary as the destructor of spdlog::logger will
    // flush anyway.
    _impl->flush();
}

const std::string& Logger::GetName() const
{
    return _name;
}

Ref<Logger> Log::GetLogger(const std::string& name)
{
    if (_sLoggers.find(name) != _sLoggers.end())
    {
        return _sLoggers[name];
    }

    return RegisterLogger({ name, DEFAULT_LOG_LEVEL, { { "stderr" } } });
}

Ref<Logger> Log::RegisterLogger(const LoggerSpecification& specification)
{
    auto logger = CreateRef<Logger>(specification);

    if (_sLoggers.find(specification.Name) != _sLoggers.end())
    {
        DGEX_CORE_ERROR("Duplicated logger with name: {0}, old logger will be replaced", specification.Name);
    }

    // [] operator will overwrite existing element.
    _sLoggers[logger->GetName()] = logger;

    return logger;
}

void Log::Init()
{
    RegisterLogger({ _DGEX_CORE_LOGGER_NAME, DEFAULT_LOG_LEVEL, { { "stderr" }, { "DgeX.log" } } });
}

DGEX_END

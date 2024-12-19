#include "DgeX/Common/Log.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

DGEX_BEGIN

Ref<spdlog::logger> Log::_sCoreLogger;
Ref<spdlog::logger> Log::_sClientLogger;

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
    case LogLevel::Warning:
        return spdlog::level::warn;
    case LogLevel::Error:
        return spdlog::level::err;
    case LogLevel::Critical:
        return spdlog::level::critical;
    case LogLevel::Disabled:
        return spdlog::level::off;
    }
    return spdlog::level::trace;
}

void Log::Init(LogLevel level, bool console, const char* filename)
{
    std::vector<spdlog::sink_ptr> logSinks;

    if (console)
    {
        auto logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        logger->set_pattern("%^[%Y-%m-%d %H:%M:%S] %=8l [%6n]: %v%$");
        logSinks.emplace_back(logger);
    }

    if (filename)
    {
        auto logger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] %8l [%6n]: %v");
        logSinks.emplace_back(logger);
    }

    _sCoreLogger = CreateRef<spdlog::logger>("DgeX", begin(logSinks), end(logSinks));
    register_logger(_sCoreLogger);

    _sClientLogger = CreateRef<spdlog::logger>("Client", begin(logSinks), end(logSinks));
    register_logger(_sClientLogger);

    auto spdLevel = LogLevelToSpdLogLevel(level);
    _sCoreLogger->set_level(spdLevel);
    _sCoreLogger->flush_on(spdLevel);
    _sClientLogger->set_level(spdLevel);
    _sClientLogger->flush_on(spdLevel);
}

DGEX_END

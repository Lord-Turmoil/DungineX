// This file has horrible performance, and will be optimized in the future.

#include "DgeX/Core/Log.h"

#include "DgeX/Utils/Xml.h"

#include <unordered_map>

DGEX_BEGIN

namespace Log
{

static Ref<Logger> sCoreLogger;
static Ref<Logger> sClientLogger;
static Ref<Logger> sDummyLogger = CreateRef<Logger>();
static std::unordered_map<std::string, Ref<LoggerSpecification>> sSpecifications;
static std::unordered_map<std::string, Ref<Logger>> sLoggers;

static constexpr char DEFAULT_COLOR_PATTERN[] = "[%Y-%m-%d %H:%M:%S] %^%8l%$ [%6n]: %v";
static constexpr char DEFAULT_PATTERN[] = "[%Y-%m-%d %H:%M:%S] %l [%n]: %v";

LogLevel LogLevelFromString(std::string level)
{
    for (auto& it : level)
    {
        it = static_cast<char>(tolower(it));
    }

    if (level == "all")
    {
        return LogLevel::All;
    }
    if (level == "fine")
    {
        return LogLevel::Fine;
    }
    if (level == "debug")
    {
        return LogLevel::Debug;
    }
    if (level == "info")
    {
        return LogLevel::Info;
    }
    if (level == "warning")
    {
        return LogLevel::Warning;
    }
    if (level == "error")
    {
        return LogLevel::Error;
    }
    if (level == "critical")
    {
        return LogLevel::Critical;
    }
    if (level == "disabled")
    {
        return LogLevel::Disabled;
    }
    return LogLevel::Invalid;
}

LogLevel LogLevelFromString(std::string level, LogLevel defaultLevel)
{
    LogLevel result = LogLevelFromString(std::move(level));
    return (result == LogLevel::Invalid) ? defaultLevel : result;
}

LoggerSink::LoggerSink(std::string file, std::string pattern) : File(std::move(file)), Pattern(std::move(pattern))
{
}

LoggerSink::LoggerSink(std::string file) : File(std::move(file)), Pattern(DEFAULT_PATTERN)
{
}

LoggerSpecification::LoggerSpecification(std::string name, LogLevel level, std::initializer_list<LoggerSink> sinks)
    : Level(level), Name(std::move(name)), Sinks(sinks)
{
}

LoggerSpecification::LoggerSpecification(std::string name, LogLevel level, const std::vector<LoggerSink>& sinks)
    : Level(level), Name(std::move(name)), Sinks(sinks)
{
}

LoggerSpecification::LoggerSpecification(std::string name, LogLevel level, const LoggerSink& sink)
    : Level(level), Name(std::move(name)), Sinks({ sink })
{
}

static void InitSpecifications()
{
    sSpecifications["Default"] =
        CreateRef<LoggerSpecification>("Default", LogLevel::Info, LoggerSink("stdout", DEFAULT_COLOR_PATTERN));

#if defined(DGEX_PUBLISH)
    sSpecifications["Core"] =
        CreateRef<LoggerSpecification>("Core", LogLevel::Warning, LoggerSink("DgeX.log", DEFAULT_FILE_PATTERN));
    sSpecifications["Client"] =
        CreateRef<LoggerSpecification>("Client", LogLevel::Warning, LoggerSink("DgeX.log", DEFAULT_FILE_PATTERN));
#elif defined(DGEX_RELEASE)
    sSpecifications["Core"] = CreateRef<LoggerSpecification>(
        "Core", LogLevel::Info,
        std::vector({ LoggerSink("stdout", DEFAULT_COLOR_PATTERN), LoggerSink("DgeX.log", DEFAULT_PATTERN) }));
    sSpecifications["Client"] = CreateRef<LoggerSpecification>(
        "Client", LogLevel::Info,
        std::vector({ LoggerSink("stdout", DEFAULT_COLOR_PATTERN), LoggerSink("DgeX.log", DEFAULT_PATTERN) }));
#else
    sSpecifications["Core"] = CreateRef<LoggerSpecification>(
        "Core", LogLevel::Debug,
        std::vector({ LoggerSink("stdout", DEFAULT_COLOR_PATTERN), LoggerSink("DgeX.log", DEFAULT_PATTERN) }));
    sSpecifications["Client"] = CreateRef<LoggerSpecification>(
        "Client", LogLevel::Debug,
        std::vector({ LoggerSink("stdout", DEFAULT_COLOR_PATTERN), LoggerSink("DgeX.log", DEFAULT_PATTERN) }));
#endif
}

static void InitImpl(const std::vector<LoggerSpecification>& specifications)
{
    sSpecifications.clear();

    InitSpecifications();

    for (auto& it : specifications)
    {
        sSpecifications[it.Name] = CreateRef<LoggerSpecification>(it);
    }

    sCoreLogger = GetLogger("Core");
    sClientLogger = GetLogger("Client");
}

void Init(const char* filename)
{
    tinyxml2::XMLDocument doc;
    if ((!Utils::Xml::OpenDocument(filename, doc)) || !doc.RootElement())
    {
        InitImpl({});
    }
    else
    {
        Init(*doc.RootElement());
    }
}

void Init(const tinyxml2::XMLElement& config)
{
    Utils::Xml::XmlElementAdapter element(&config);
    if (!element.Attribute("enabled", true))
    {
        // Don't initialize logging if explicitly disabled.
    }

    if (!element.GoToFirstChild())
    {
        // Default logging if no specifications are provided.
        InitImpl({});
    }

    std::vector<LoggerSpecification> specifications;
    do
    {
        LoggerSpecification spec;
        spec.Name = element.Attribute("name", "");
        spec.Level = LogLevelFromString(element.Attribute("level", "info"), LogLevel::Info);

        Utils::Xml::XmlElementAdapter child = element;
        if (child.GoToFirstChild())
        {
            do
            {
                LoggerSink sink;
                sink.File = child.Attribute("file", "stdout");
                sink.Pattern = child.Attribute("pattern", DEFAULT_PATTERN);
                spec.Sinks.emplace_back(std::move(sink));
            } while (child.GoToNextSibling());
        }

        if (!(spec.Name.empty() || spec.Sinks.empty()))
        {
            specifications.emplace_back(std::move(spec));
        }
    } while (element.GoToNextSibling());

    InitImpl(specifications);
}

void Init(const std::vector<LoggerSpecification>& specifications)
{
    InitImpl(specifications);
}

Ref<Logger> GetCoreLogger()
{
    return sCoreLogger ? sCoreLogger : sDummyLogger;
}

Ref<Logger> GetClientLogger()
{
    return sClientLogger ? sClientLogger : sDummyLogger;
}

Ref<Logger> GetLogger(const std::string& name)
{
    if (auto it = sLoggers.find(name); it != sLoggers.end())
    {
        return it->second;
    }

    auto it = sSpecifications.find(name);
    if (it == sSpecifications.end())
    {
        it = sSpecifications.find("Default");
        if (it == sSpecifications.end())
        {
            // logger not enabled
            return sDummyLogger;
        }
    }
    Ref<Logger> logger = CreateRef<Logger>(*(it->second));
    sLoggers[name] = logger;

    return logger;
}

} // namespace Log

DGEX_END

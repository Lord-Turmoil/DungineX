#pragma once

#include "DgeX/Core/Base.h"

#include <spdlog/spdlog.h>
#include <tinyxml2.h>

DGEX_BEGIN

namespace Log
{

struct LoggerSpecification;

class Logger
{
public:
    Logger() = default;
    Logger(const LoggerSpecification& config);
    ~Logger();

public:
    // I prefer Uppercase for the first letter of the function name, so I
    // wrap the original functions from spdlog.

    template <typename T> void Trace(const T& msg)
    {
        if (_impl)
        {
            _impl->trace(msg);
        }
    }

    template <typename... Args> void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (_impl)
        {
            _impl->trace(fmt, std::forward<Args>(args)...);
        }
    }

    template <typename T> void Debug(const T& msg)
    {
        if (_impl)
        {
            _impl->debug(msg);
        }
    }

    template <typename... Args> void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (_impl)
        {
            _impl->debug(fmt, std::forward<Args>(args)...);
        }
    }

    template <typename T> void Info(const T& msg)
    {
        if (_impl)
        {
            _impl->info(msg);
        }
    }

    template <typename... Args> void Info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (_impl)
        {
            _impl->info(fmt, std::forward<Args>(args)...);
        }
    }

    template <typename T> void Warn(const T& msg)
    {
        if (_impl)
        {
            _impl->warn(msg);
        }
    }

    template <typename... Args> void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (_impl)
        {
            _impl->warn(fmt, std::forward<Args>(args)...);
        }
    }

    template <typename T> void Error(const T& msg)
    {
        if (_impl)
        {
            _impl->error(msg);
        }
    }

    template <typename... Args> void Error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (_impl)
        {
            _impl->error(fmt, std::forward<Args>(args)...);
        }
    }

    template <typename T> void Critical(const T& msg)
    {
        if (_impl)
        {
            _impl->critical(msg);
        }
    }

    template <typename... Args> void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (_impl)
        {
            _impl->critical(fmt, std::forward<Args>(args)...);
        }
    }

private:
    void _Init(const LoggerSpecification& spec);

private:
    Ref<spdlog::logger> _impl;
};

} // namespace Log

DGEX_END

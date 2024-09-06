#pragma once

#include <filesystem>

#include "DgeX/Common/Log.h"

#ifdef DGEX_ENABLE_ASSERT
#define DGEX_DEBUG_BREAK() __debugbreak()
#else
#define DGEX_DEBUG_BREAK()
#endif

#ifdef DGEX_ENABLE_ASSERT

#ifdef _DGEX_CORE_
#define _DGEX_ASSERT_LOGGER DGEX_EXPAND_MACRO(DGEX_CORE_CRITICAL)
#else
#define _DGEX_ASSERT_LOGGER DGEX_EXPAND_MACRO(DGEX_LOG_CRITICAL)
#endif

#define _DGEX_ASSERT_IMPL(expression, msg, ...)                                                                        \
    {                                                                                                                  \
        if (!(expression))                                                                                             \
        {                                                                                                              \
            _DGEX_ASSERT_LOGGER(msg, __VA_ARGS__);                                                                     \
            DGEX_DEBUG_BREAK();                                                                                        \
        }                                                                                                              \
    }

#define _DGEX_ASSERT_WITH_MSG(expression, ...)                                                                         \
    _DGEX_ASSERT_IMPL(expression, "Assertion '{0}' failed at {1}:{2} with message: {3}",                               \
                      DGEX_STRINGIFY_MACRO(expression), std::filesystem::path(__FILE__).filename().string(), __LINE__, \
                      __VA_ARGS__)

#define _DGEX_ASSERT_NO_MSG(expression)                                                                                \
    _DGEX_ASSERT_IMPL(expression, "Assertion '{0}' failed at {1}:{2}", DGEX_STRINGIFY_MACRO(expression),               \
                      std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define _DGEX_ASSERT_MACRO_NAME(_1, _2, macro, ...) macro
#define _DGEX_ASSERT_MACRO(...)                                                                                        \
    DGEX_EXPAND_MACRO(_DGEX_ASSERT_MACRO_NAME(__VA_ARGS__, _DGEX_ASSERT_WITH_MSG, _DGEX_ASSERT_NO_MSG))

// Only support one string message (expression, message) or (expression)
#define DGEX_ASSERT(...) DGEX_EXPAND_MACRO(_DGEX_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
#define DGEX_ASSERT(...)
#endif

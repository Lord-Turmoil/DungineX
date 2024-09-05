#pragma once

#include <filesystem>

#include "DgeX/Common/Log.h"

#ifdef DGEX_ENABLE_ASSERT
    #define DGEX_DEBUG_BREAK() __debugbreak()
#else
	#define DGEX_DEBUG_BREAK()
#endif

#ifdef DGEX_ENABLE_ASSERT
	#define DGEX_INTERNAL_ASSERT_IMPL(expression, msg, ...) {  \
            if (!(expression)) {                               \
                DGEX_LOG_CRITICAL(msg, __VA_ARGS__);           \
                DGEX_DEBUG_BREAK();                            \
            }                                                  \
        }

    #define DGEX_INTERNAL_ASSERT_WITH_MSG(expression, ...)               \
        DGEX_INTERNAL_ASSERT_IMPL(                                       \
            expression,                                                  \
            "Assertion '%s' failed at %s:%d with message: %s",           \
            DGEX_STRINGIFY_MACRO(expression),                            \
            std::filesystem::path(__FILE__).filename().string().c_str(), \
            __LINE__,                                                    \
            __VA_ARGS__)

	#define DGEX_INTERNAL_ASSERT_NO_MSG(expression)                      \
	    DGEX_INTERNAL_ASSERT_IMPL(                                       \
            expression,                                                  \
            "Assertion '%s' failed at %s:%d",                            \
            DGEX_STRINGIFY_MACRO(expression),                            \
            std::filesystem::path(__FILE__).filename().string().c_str(), \
            __LINE__)

    #define DGEX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define DGEX_INTERNAL_ASSERT_GET_MACRO(...) DGEX_EXPAND_MACRO(DGEX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, DGEX_INTERNAL_ASSERT_WITH_MSG, DGEX_INTERNAL_ASSERT_NO_MSG))

    // Only support one string message (expression, message) or (expression)
	#define DGEX_ASSERT(...) DGEX_EXPAND_MACRO(DGEX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
    #define DGEX_ASSERT(...)
#endif

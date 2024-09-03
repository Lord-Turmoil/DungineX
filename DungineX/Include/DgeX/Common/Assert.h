#pragma once

#include <filesystem>

#include "DgeX/Common/Log.h"

#ifdef DGEX_ENABLE_ASSERT
    #define DGEX_DEBUG_BREAK() __debugbreak()
#else
	#define DGEX_DEBUG_BREAK()
#endif

#ifdef DGEX_ENABLE_ASSERT
	#define DGEX_INTERNAL_ASSERT_IMPL(expression, msg, ...) {   \
            if (!(expression)) {                                \
                DGEX_LOG_CRITICAL(msg, __VA_ARGS__);             \
                DGEX_DEBUG_BREAK();                             \
            }                                                   \
        }

	#define DGEX_INTERNAL_ASSERT(expression)     \
	    DGEX_INTERNAL_ASSERT_IMPL(               \
            expression,                          \
            L"Assertion '%s' failed at %s:%d",    \
            DGEX_STRINGIFY_MACRO(expression),    \
            std::filesystem::path(__FILE__).filename().wstring().c_str(),   \
            __LINE__)

	#define DGEX_ASSERT(expression) DGEX_INTERNAL_ASSERT(expression)
#else
    #define DGEX_ASSERT(expression)
#endif

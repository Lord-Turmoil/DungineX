/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Assert.h                                  *
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
 * Assertion support.                                                         *
 ******************************************************************************/

#pragma once

#include "DgeX/Utils/Log.h"
#include "DgeX/Utils/Macros.h"

#include <filesystem>

#ifdef DGEX_ENABLE_ASSERT
#define DGEX_DEBUG_BREAK() __debugbreak()
#else
#define DGEX_DEBUG_BREAK()
#endif

#ifdef DGEX_ENABLE_ASSERT

#define _DGEX_ASSERT_LOGGER DGEX_EXPAND_MACRO(DGEX_CORE_CRITICAL)

#define _DGEX_ASSERT_IMPL(EXPRESSION, MESSAGE, ...)                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(EXPRESSION))                                                                                             \
        {                                                                                                              \
            _DGEX_ASSERT_LOGGER(MESSAGE, __VA_ARGS__);                                                                 \
            DGEX_DEBUG_BREAK();                                                                                        \
        }                                                                                                              \
    } while (0)

#define _DGEX_ASSERT_WITH_MSG(EXPRESSION, ...)                                                                         \
    _DGEX_ASSERT_IMPL(EXPRESSION, "Assertion '{0}' failed at {1}:{2}: {3}", DGEX_STRINGIFY_MACRO(EXPRESSION),          \
                      std::filesystem::path(__FILE__).filename().string(), __LINE__, __VA_ARGS__)

#define _DGEX_ASSERT_NO_MSG(EXPRESSION)                                                                                \
    _DGEX_ASSERT_IMPL(EXPRESSION, "Assertion '{0}' failed at {1}:{2}", DGEX_STRINGIFY_MACRO(EXPRESSION),               \
                      std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define _DGEX_ASSERT_MACRO_NAME(_1, _2, MACRO, ...) MACRO
#define _DGEX_ASSERT_MACRO(...)                                                                                        \
    DGEX_EXPAND_MACRO(_DGEX_ASSERT_MACRO_NAME(__VA_ARGS__, _DGEX_ASSERT_WITH_MSG, _DGEX_ASSERT_NO_MSG))

/**
 * This does not support arguments.
 *
 * DGEX_ASSERT(a > b);
 * DGEX_ASSERT(a > b, "a must not greater than b");
 */
#define DGEX_ASSERT(...) DGEX_EXPAND_MACRO(_DGEX_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
#define DGEX_ASSERT(...)
#endif

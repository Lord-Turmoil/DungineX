#pragma once

#include "DgeX/Core/Log.h"

#include <filesystem>

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
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(expression))                                                                                             \
        {                                                                                                              \
            _DGEX_ASSERT_LOGGER(msg, __VA_ARGS__);                                                                     \
            DGEX_DEBUG_BREAK();                                                                                        \
        }                                                                                                              \
    } while (0)

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

// Common assert messages
#define DGEX_MSG_GLAD_INIT_FAILED "Failed to initialize Glad!"
#define DGEX_MSG_GLFW_INIT_FAILED "Failed to initialize GLFW!"

#define DGEX_MSG_UNKNOWN_PLATFORM "Unknown platform!"

#define DGEX_MSG_APPLICATION_ALREADY_CREATED "Application already created!"
#define DGEX_MSG_APPLICATION_RUNNING         "Application is already running!"
#define DGEX_MSG_NO_SPLASH_INTERFACE         "Splash interface not found!"
#define DGEX_MSG_NO_MAIN_INTERFACE           "Main interface not found!"

#define DGEX_MSG_INVALID_FRAME_BUFFER_SIZE "Invalid frame buffer size!"

#define DGEX_MSG_CANNOT_OPEN_FILE "Cannot open file '{0}'!"
#define DGEX_MSG_CANNOT_READ_FILE "Cannot read file '{0}'!"

#define DGEX_MSG_SHADER_SYNTAX_ERROR  "Shader syntax error near '{0}'!"
#define DGEX_MSG_SHADER_COMPILE_ERROR "Failed to compile shader '{0}'!"
#define DGEX_MSG_SHADER_LINK_ERROR    "Failed to link shader '{0}'!"
#define DGEX_MSG_SHADER_LOAD_ERROR    "Failed to load shader '{0}'!"

#define DGEX_MSG_IMAGE_INVALID_FORMAT "Invalid image format!"
#define DGEX_MSG_TEXTURE_DATA_SIZE    "Texture data size does not match the specification!"

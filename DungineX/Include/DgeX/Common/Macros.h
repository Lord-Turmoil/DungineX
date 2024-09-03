#pragma once

#define DUNGEON_ENGINE_X

// Define the DungineX version
#define DGEX_VERSION_MAJOR 0
#define DGEX_VERSION_MINOR 1
#define DGEX_VERSION_PATCH 0

// Define the DungineX debug flag
#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG
#endif

#ifdef DGEX_DEBUG
#define DGEX_ENABLE_ASSERT
#endif

// Only do logging with this macro defined
#define DGEX_ENABLE_TRACE

#define DGEX_BEGIN namespace DgeX {
#define DGEX_END }

// Define backend platform
#define DGEX_GLFW (1)
#define DGEX_EASYX (!DGEX_GLFW)

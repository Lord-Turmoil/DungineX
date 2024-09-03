#pragma once

#include "DgeX/Core/Macros.h"

// Define the DungineX version
#define DGEX_VERSION_MAJOR 0
#define DGEX_VERSION_MINOR 1
#define DGEX_VERSION_PATCH 0

// clang-format off
#define DGEX_VERSION_STRING                                                                                            \
    DGEX_STRINGIFY(DGEX_VERSION_MAJOR)                                                                                 \
    "."                                                                                                                \
    DGEX_STRINGIFY(DGEX_VERSION_MINOR)                                                                                 \
    "."                                                                                                                \
    DGEX_STRINGIFY(DGEX_VERSION_PATCH)
// clang-format on

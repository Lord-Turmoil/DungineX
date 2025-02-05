#pragma once

#include "DgeX/Core/Macros.h"

#define DPHX_VERSION_MAJOR 0
#define DPHX_VERSION_MINOR 1
#define DPHX_VERSION_PATCH 1

#define DPHX_VERSION DGEX_VERSION(DPHX_VERSION_MAJOR, DPHX_VERSION_MINOR, DPHX_VERSION_PATCH)

// clang-format off
#define DGEX_VERSION_STRING                                                                                            \
    DGEX_STRINGIFY(DGEX_VERSION_MAJOR)                                                                                 \
    "."                                                                                                                \
    DGEX_STRINGIFY(DGEX_VERSION_MINOR)                                                                                 \
    "."                                                                                                                \
    DGEX_STRINGIFY(DGEX_VERSION_PATCH)
// clang-format on

#pragma once

#include "DgeX/Core/Macros.h"

#define DUNGINE_PHYSICS

// Use high precision floating point numbers, i.e. double
#define DPHX_HIGH_PRECISION

#define DPHX_BEGIN                                                                                                     \
    DGEX_BEGIN namespace Physics                                                                                       \
    {

#define DPHX_END                                                                                                       \
    }                                                                                                                  \
    }

#define DPHX DGEX Physics::

#pragma once

#define DUNGINE_PHYSICS

#define DPHX_VERSION_MAJOR 0
#define DPHX_VERSION_MINOR 1
#define DPHX_VERSION_PATCH 0

// Use high precision floating point numbers, i.e. double
#define DPHX_HIGH_PRECISION

#define DPHX_BEGIN                                                                                                     \
    DGEX_BEGIN namespace Physics                                                                                       \
    {

#define DPHX_END                                                                                                       \
    }                                                                                                                  \
    }

#define DPHX DGEX Physics::

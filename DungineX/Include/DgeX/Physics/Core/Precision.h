#pragma once

#include "DgeX/Physics/Common.h"

#include <limits>

DPHX_BEGIN

#ifdef DPHX_HIGH_PRECISION
typedef double real_t;
constexpr real_t REAL_EPSILON = DBL_EPSILON;
#else
typedef float real_t;
constexpr real_t REAL_EPSILON = FLT_EPSILON;
#endif
constexpr real_t MAX_REAL = std::numeric_limits<real_t>::max();

DPHX_END

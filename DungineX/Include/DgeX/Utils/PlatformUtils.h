#pragma once

#include "DgeX/Common/Base.h"
#include "DgeX/Core/DeltaTime.h"

DGEX_BEGIN

namespace Time
{

/**
 * @brief Get elapsed time from the start of the application
 * @return elapsed time in seconds
 */
timestamp_t GetTimestamp();

} // namespace Time

DGEX_END

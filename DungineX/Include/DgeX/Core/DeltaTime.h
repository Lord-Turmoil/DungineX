#pragma once

#include "DgeX/Common/Base.h"

DGEX_BEGIN

#ifdef DGEX_HIGH_PRECISION_TIME
typedef double timestamp_t;
#else
typedef float timestamp_t;
#endif

class DeltaTime
{
public:
    DeltaTime(timestamp_t time = 0.0f) : _time(time)
    {
    }

    operator timestamp_t() const
    {
        return _time;
    }

    timestamp_t Seconds() const
    {
        return _time;
    }

    timestamp_t Milliseconds() const
    {
        return _time * 1000.0f;
    }

private:
    timestamp_t _time;
};

DGEX_END

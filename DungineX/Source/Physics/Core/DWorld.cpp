#include "DgeX/Physics/Core/DWorld.h"

DPHX_BEGIN

void DWorld::Step(real_t delta)
{
    if (!_adaptive)
    {
        for (real_t i = 0.0; i < delta; i += _step)
        {
            _Step(_step);
        }

        return;
    }

    // Dynamically update the simulation rate.
    if (delta > _desiredFrameTime * _threshold)
    {
        _adaptiveFrameCount++;
        if (_adaptiveFrameCount >= 5)
        {
            // If we hit the threshold for 5 times in a row, we will reduce the
            // simulation iteration.
            _adaptiveFrameTime = Math::Max(_step, _adaptiveFrameTime * 0.9);
            if (_adaptiveFrameCount == 5)
            {
                DGEX_CORE_WARN("Performance hit detected, reducing simulation iteration to {0} FPS",
                               1.0 / (_desiredFrameTime * _desiredFrameTime / _adaptiveFrameTime));
            }
        }
    }
    else
    {
        if (_adaptiveFrameCount > 0)
        {
            _adaptiveFrameTime = Math::Min(_desiredFrameTime, _adaptiveFrameTime * 1.05);
        }
        _adaptiveFrameCount = 0;
    }

    for (real_t i = 0.0; i < _adaptiveFrameTime; i += _step)
    {
        _Step(_step);
    }
}

void DWorld::SetSimulationRate(int fps)
{
    _step = 1.0 / static_cast<real_t>(fps);
}

void DWorld::EnableAdaptive(int fps, real_t threshold)
{
    _adaptive = true;
    _adaptiveFrameTime = _desiredFrameTime = 1.0 / static_cast<real_t>(fps);
    _threshold = threshold;
}

void DWorld::DisableAdaptive()
{
    _adaptive = false;
}

DPHX_END

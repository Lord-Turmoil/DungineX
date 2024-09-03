#pragma once

#include "DgeX/Physics/Core/Precision.h"

DPHX_BEGIN

class MassWorld
{
public:
    virtual ~MassWorld() = default;

    /**
     * @brief Step the simulation.
     * @note
     * This method will call the _Step method which should be implemented
     * by the derived class.
     * @param delta last frame time
     */
    void StepForward(real_t delta);

    /**
     * @brief Set the desired simulation rate.
     * @param fps how many simulations in one second
     */
    void SetSimulationRate(int fps);

    /**
     * @brief Enable adaptive simulation to reduce performance cost.
     * @note By default, adaptive
     * @param fps fps of the application
     * @param threshold the threshold to enable adaptive simulation
     *                  if the frame time is longer than the
     *                  desired time * threshold, will reduce simulation
     *                  iteration to compensate the performance.
     */
    void EnableAdaptive(int fps, real_t threshold = 1.2);
    void DisableAdaptive();

protected:
    virtual void Step(real_t delta) = 0;

private:
    /**
     * @brief The desired simulation time step interval;
     * @note In every update, physics will be updated multiple times by this step.
     * @note By default, the simulation goes with 1000 steps per second.
     */
    real_t _step = 1.0 / 1000.0;

    /**
     * @brief Whether enable adaptive frame rate.
     * @note By default, disabled.
     */
    bool _adaptive = false;

    real_t _desiredFrameTime = 0.0;
    real_t _adaptiveFrameTime = 0.0;
    real_t _threshold = 1.1;
    uint32_t _adaptiveFrameCount = 0;
};

DPHX_END

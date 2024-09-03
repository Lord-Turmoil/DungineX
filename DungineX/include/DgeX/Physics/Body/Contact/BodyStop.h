#pragma once

#include "DgeX/Physics/Body/Contact/BodyLink.h"

DPHX_BEGIN

/**
 * @brief Constraint the minimum distance between two bodies.
 */
class BodyStop : public BodyLink
{
public:
    BodyStop() : MinLength(0)
    {
    }

    BodyStop(real_t minLength) : MinLength(minLength)
    {
    }

    ~BodyStop() override = default;

    uint32_t AddContact(BodyContact* contact, uint32_t limit) const override;

public:
    real_t MinLength;
};

DPHX_END

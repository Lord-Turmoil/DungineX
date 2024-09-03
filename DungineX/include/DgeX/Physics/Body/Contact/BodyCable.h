#pragma once

#include "DgeX/Physics/Body/Contact/BodyLink.h"

DPHX_BEGIN

/**
 * @brief Restrict the maximum distance between two bodies.
 */
class BodyCable : public BodyLink
{
public:
    BodyCable() : MaxLength(0)
    {
    }

    BodyCable(real_t maxLength) : MaxLength(maxLength)
    {
    }

    ~BodyCable() override = default;

    uint32_t AddContact(BodyContact* contact, uint32_t limit) const override;

public:
    real_t MaxLength;
};

DPHX_END

#pragma once

#include "DgeX/Physics/Body/Contact/BodyLink.h"

DPHX_BEGIN

/**
 * @brief Rod constraint the distance between two bodies.
 */
class BodyRod : public BodyLink
{
public:
    BodyRod() : Length(0)
    {
    }

    BodyRod(real_t length) : Length(length)
    {
    }

    ~BodyRod() override = default;

    uint32_t AddContact(BodyContact* contact, uint32_t limit) const override;

public:
    real_t Length;
};

/**
 * @brief
 * Joint connect two bodies and make sure they don't separate.
 * i.e. The two connection points will be kept at the same position.
 *
 * @note
 * This is actually an alias for BodyRod with zero length, so... you
 * still have access to Length. :(
 */
class BodyJoint : public BodyRod
{
public:
    BodyJoint() : BodyRod(0)
    {
    }

    ~BodyJoint() override = default;
};

DPHX_END

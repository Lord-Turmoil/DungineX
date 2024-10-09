#pragma once

#include "DgeX/Physics/Core/Precision.h"

DPHX_BEGIN

class BodyContact;

/**
 * @brief
 * Base class for contact generators that generate contacts between rigid bodies.
 */
class BodyContactGenerator
{
public:
    virtual ~BodyContactGenerator() = default;

    /**
     * @brief
     * Add contacts to the contact list with the given limit.
     * The contact should be a pointer in an array of contacts and point to
     * the first available contact slot.
     *
     * @note
     * The limit is the maximum number of contacts that can be written, it
     * is greater than or equal to 1.
     */
    virtual uint32_t AddContact(BodyContact* contact, uint32_t limit) const = 0;
};

DPHX_END

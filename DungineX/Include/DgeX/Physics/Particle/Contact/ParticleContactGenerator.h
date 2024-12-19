#pragma once

#include "DgeX/Library/IntrusiveList.h"
#include "DgeX/Physics/Common.h"

DPHX_BEGIN

class ParticleContact;

/**
 * @brief
 * Base class for contact generators that generate contacts between particles.
 */
class ParticleContactGenerator : public IntrusiveListNode
{
public:
    ~ParticleContactGenerator() override = default;

    /**
     * @brief
     * Add contacts to the contact list with the given limit.
     * The contact should be a pointer in an array of contacts.
     *
     * @note
     * The limit is the maximum number of contacts that can be written, it
     * is greater than or equal to 1.
     */
    virtual uint32_t AddContact(ParticleContact* contact, uint32_t limit) const = 0;
};

/**
 * @brief A registry to hold all particle contact generators.
 */
class ParticleContactRegistry
{
public:
    void Add(ParticleContactGenerator* contactGenerator);
    void Remove(ParticleContactGenerator* contactGenerator);
    void Clear();

    uint32_t AddContact(ParticleContact* contacts, uint32_t limit) const;

private:
    IntrusiveList<ParticleContactGenerator> _contactGenerators;
};

DPHX_END

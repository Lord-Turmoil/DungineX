#include "Firework.h"

#include "DgeX/Physics/Utils/Utils.h"

void FireworkSpecification::Create(Firework* firework, Firework* parent) const
{
    DGEX_ASSERT(parent, "Parent mustn't be null");
    _Create(firework, &parent->GetPosition(), &parent->GetVelocity());
    firework->SetIteration(parent->GetIteration() + 1);
}

void FireworkSpecification::Create(Firework* firework, const Vector3& position) const
{
    _Create(firework, &position, nullptr);
}

void FireworkSpecification::_Create(Firework* firework, const Vector3* position, const Vector3* velocity) const
{
    DGEX_ASSERT(firework, "Firework mustn't be null");
    DGEX_ASSERT(position, "Position mustn't be null");

    firework->SetType(Type);
    firework->SetAge(DgeX::Utils::Random::RandomNumber(MinAge, MaxAge));

    Vector3 initialVelocity = velocity ? *velocity : Vector3::Zero;
    firework->SetPosition(*position);

    initialVelocity += Physics::Utils::RandomVector(MinVelocity, MaxVelocity);
    firework->SetVelocity(initialVelocity);

    firework->SetMass(1.0);
    firework->SetDamping(Damping);

    firework->SetAcceleration(Gravity);

    // prevent dirty data
    firework->ClearAccumulatedForce();
}

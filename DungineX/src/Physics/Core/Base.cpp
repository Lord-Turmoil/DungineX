#include "DgeX/Physics/Core/Base.h"

#include "DgeX/Common/Assert.h"

DPHX_BEGIN

void DObject::SetMass(real_t mass)
{
    DGEX_ASSERT(mass > 0);
    SetInverseMass(static_cast<real_t>(1) / mass);
}

real_t DObject::GetMass() const
{
    if (HasInfiniteMass())
    {
        return MAX_REAL;
    }
    return static_cast<real_t>(1) / _inverseMass;
}

DPHX_END

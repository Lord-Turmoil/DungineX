#pragma once

#include "DgeX/Core/Base.h"

DGEX_BEGIN

/**
 * @brief Make the object cloneable.
 */
template <typename T> class Cloneable
{
public:
    virtual ~Cloneable() = default;

    /**
     * @brief Clone the object.
     * @return A new instance of the object.
     */
    virtual Ref<T> Clone() const
    {
        return CreateRef<T>(*this);
    }
};

DGEX_END

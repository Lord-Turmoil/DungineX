/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Types.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 1, 2025                              *
 *                                                                            *
 *                    Last Update : June 18, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Common type declarations.                                                  *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <memory>

DGEX_BEGIN

// ============================================================================
// Smart Pointer
// ----------------------------------------------------------------------------

/**
 * We use Scope for std::unique_ptr.
 */
template <typename T> using Scope = std::unique_ptr<T>;

template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * We use Ref for std::shared_ptr.
 */
template <typename T> using Ref = std::shared_ptr<T>;

template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/**
 * Just an alias for raw pointers.
 */
template <typename T> using Ptr = T*;

// ============================================================================
// Shapes
// ----------------------------------------------------------------------------

template <typename T> struct TRect
{
    T X;
    T Y;
    T Width;
    T Height;

    explicit TRect(T x = 0, T y = 0, T width = 0, T height = 0) : X(x), Y(y), Width(width), Height(height)
    {
    }
};

using Rect = TRect<int>;

DGEX_END

// ============================================================================
// Other Implementations
// ----------------------------------------------------------------------------

#include <DgeX/Impl/Expected.h>

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
#include "Macros.h"

#include <memory>
#include <type_traits>

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

// ============================================================================
// Type Alias Helper
// ----------------------------------------------------------------------------

/**
 * @brief Cast a value, especially an enum value, to its underlying type.
 *
 * @tparam T Auto-deducted type.
 * @param value Value to convert to its literal type.
 * @return The value of its underlying type.
 */
template <typename T> constexpr auto L(T value) noexcept
{
    return static_cast<std::underlying_type_t<T>>(value);
}

// ============================================================================
// Shapes
// ----------------------------------------------------------------------------

template <typename T> struct RectT
{
    T X;
    T Y;
    T Width;
    T Height;

    explicit RectT(T x = 0, T y = 0, T width = 0, T height = 0) : X(x), Y(y), Width(width), Height(height)
    {
    }
};

using Rect = RectT<int>;
using FRect = RectT<float>;

template <typename T> struct PointT
{
    T X;
    T Y;

    explicit PointT(T x = 0, T y = 0) : X(x), Y(y)
    {
    }
};

using Point = PointT<int>;
using FPoint = PointT<float>;

// ============================================================================
// Styles
// ----------------------------------------------------------------------------

struct TextureStyle
{
    float Scale = 1.0f;  // scale, 1.0 for no scale
    float Degree = 0.0f; // rotation in degree
    uint8_t Alpha = 255; // alpha value, 0 ~ 255
    bool FlipX : 1;      // flip horizontally
    bool FlipY : 1;      // flip vertically
};

struct TextureAnchor
{
    int X;
    int Y;
};

using TextFlags = unsigned char;

// clang-format off
enum class TextFlag : unsigned char
{
    AlignLeft   = DGEX_BIT(0),
    AlignRight  = DGEX_BIT(1),
    AlignCenter = DGEX_BIT(2),
    Overflow    = DGEX_BIT(3)
};

// clang-format on

DGEX_END

// ============================================================================
// Other Implementations
// ----------------------------------------------------------------------------

#include <DgeX/Impl/Expected.h>

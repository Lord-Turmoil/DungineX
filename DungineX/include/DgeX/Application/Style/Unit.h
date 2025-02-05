#pragma once

#include "DgeX/Core/Macros.h"

#include <cstdint>

DGEX_BEGIN

/**
 * @brief Unit for property value related to length.
 * @paragraph
 * The representation of the unit is as follows:
 * - Pixel: px (default, can be omitted, e.g. 10, 10px)
 * - Percent: % (e.g. 50%)
 */
// clang-format off
struct Unit
{
    enum class UnitType : uint8_t
    {
        Pixel,
        Percent,
        Auto
    };

    UnitType Type;
    float Value;

    Unit() : Type(UnitType::Auto), Value(0.0f) {}
    Unit(UnitType type, float value) : Type(type), Value(value) {}

    static Unit FromString(const char* str);

    bool IsPixel() const { return Type == UnitType::Pixel; }
    bool IsPercent() const { return Type == UnitType::Percent; }
    bool IsAuto() const { return Type == UnitType::Auto; }

    float ToPixel(float parentSize) const;

    bool operator==(const Unit& other) const;
    bool operator!=(const Unit& other) const;

    Unit& operator*=(float scalar);
    Unit& operator/=(float scalar);

    friend Unit operator*(const Unit& unit, float scalar);
    friend Unit operator*(float scalar, const Unit& unit);
    friend Unit operator/(const Unit& unit, float scalar);
};

// clang-format on

DGEX_END

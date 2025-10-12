/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Color.h                                   *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 2, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Color definition.                                                          *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <cstdint>
#include <string>

DGEX_BEGIN

#define DGEX_COLOR_OPAQUE      255
#define DGEX_COLOR_TRANSPARENT 0

enum class ColorFormats
{
    RGB,  // rgb(r, g, b)
    RGBA, // rgba(r, g, b, a)
    HRGB, // #RRGGBB
    HARGB // #AARRGGBB
};

struct Color
{
    DGEX_DATA static const Color Black;
    DGEX_DATA static const Color Blue;
    DGEX_DATA static const Color Green;
    DGEX_DATA static const Color Cyan;
    DGEX_DATA static const Color Red;
    DGEX_DATA static const Color Magenta;
    DGEX_DATA static const Color Brown;
    DGEX_DATA static const Color LightGray;
    DGEX_DATA static const Color DarkGray;
    DGEX_DATA static const Color LightBlue;
    DGEX_DATA static const Color LightGreen;
    DGEX_DATA static const Color LightCyan;
    DGEX_DATA static const Color LightRed;
    DGEX_DATA static const Color LightMagenta;
    DGEX_DATA static const Color Yellow;
    DGEX_DATA static const Color White;

    DGEX_DATA static const Color Transparent;

    uint8_t R, G, B, A;

    DGEX_API Color();
    DGEX_API Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = DGEX_COLOR_OPAQUE);

    /**
     * @brief Construct color from string.
     *
     * The string can be in formats of:
     * - #RRGGBB
     * - #AARRGGBB
     * - rgb(r, g, b)
     * - rgba(r, g, b, a)
     *
     * @param value Color value.
     */
    DGEX_API Color(const char* value);

    /**
     * @brief Get color from hex number.
     *
     * Hex color in format of: 0xAARRGGBB
     *
     * @param color Color in hex format.
     * @return Color.
     */
    DGEX_API static Color FromHex(uint32_t color);

    DGEX_API uint32_t ToHex() const;
    DGEX_API std::string ToString() const;
    DGEX_API std::string ToString(ColorFormats format) const;

    /**
     * @brief Apply additional opacity to the color.
     *
     * @param opacity Opacity.
     * @return Color with additional opacity.
     */
    DGEX_API Color ApplyOpacity(float opacity) const;

    DGEX_API bool operator==(const Color& other) const;

    DGEX_API Color& operator*=(float scalar);
    DGEX_API Color& operator/=(float scalar);

    // Although I don't think adding or subtracting colors make sense,
    // they are somehow required for easing but just provide them for completeness.
    // These two will not change alpha channel.
    DGEX_API Color operator+(const Color& other) const;
    DGEX_API Color operator-(const Color& other) const;

    DGEX_API friend Color operator*(const Color& color, float scalar);
    DGEX_API friend Color operator*(float scalar, const Color& color);
    DGEX_API friend Color operator/(const Color& color, float scalar);
};

DGEX_END

// Definition of math functions.

#include "DgeX/Utils/Math.h"

DGEX_BEGIN

namespace Math
{

template <>
inline Color Cos<Color>(Color x)
{
    return { static_cast<uint8_t>(std::cos(static_cast<float>(x.R)) * 255),
             static_cast<uint8_t>(std::cos(static_cast<float>(x.G)) * 255),
             static_cast<uint8_t>(std::cos(static_cast<float>(x.B)) * 255), x.A };
}

template <>
inline Color Sin<Color>(Color x)
{
    return { static_cast<uint8_t>(std::sin(static_cast<float>(x.R)) * 255),
             static_cast<uint8_t>(std::sin(static_cast<float>(x.G)) * 255),
             static_cast<uint8_t>(std::sin(static_cast<float>(x.B)) * 255), x.A };
}

template <>
inline Color Tan(Color x)
{
    return { static_cast<uint8_t>(std::tan(static_cast<float>(x.R)) * 255),
             static_cast<uint8_t>(std::tan(static_cast<float>(x.G)) * 255),
             static_cast<uint8_t>(std::tan(static_cast<float>(x.B)) * 255), x.A };
}

// What, power of color? I don't think it should be used...
template <>
inline Color Pow(Color x, float y)
{
    return { static_cast<uint8_t>(std::pow(static_cast<float>(x.R) / 255.0f, y) * 255),
             static_cast<uint8_t>(std::pow(static_cast<float>(x.G) / 255.0f, y) * 255),
             static_cast<uint8_t>(std::pow(static_cast<float>(x.B) / 255.0f, y) * 255), x.A };
}

template <>
inline Color Pow2(Color x)
{
    return Pow(x, 2.0f);
}

template <>
inline Color Pow3(Color x)
{
    return Pow(x, 3.0f);
}

template <>
inline Color Pow4(Color x)
{
    return Pow(x, 4.0f);
}

template <>
inline Color Pow5(Color x)
{
    return Pow(x, 5.0f);
}

template <>
inline Color Abs(Color x)
{
    return x;
}

template <>
inline Color Sqrt(Color x)
{
    return { static_cast<uint8_t>(std::sqrt(static_cast<float>(x.R))),
             static_cast<uint8_t>(std::sqrt(static_cast<float>(x.G))),
             static_cast<uint8_t>(std::sqrt(static_cast<float>(x.B))), x.A };
};

} // namespace Math

DGEX_END

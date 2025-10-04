/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Color.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
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

    DGEX_API bool operator==(const Color& other) const;

    DGEX_API Color& operator*=(float scalar);
    DGEX_API Color& operator/=(float scalar);

    DGEX_API friend Color operator*(const Color& color, float scalar);
    DGEX_API friend Color operator*(float scalar, const Color& color);
    DGEX_API friend Color operator/(const Color& color, float scalar);
};

DGEX_END

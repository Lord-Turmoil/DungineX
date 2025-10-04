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

#include "DgeX/Renderer/Color.h"

#include "DgeX/Utils/Math.h"

#include <sstream>

DGEX_BEGIN

// clang-format off
const Color Color::Black        = FromHex(0xFF000000);
const Color Color::Blue         = FromHex(0xFF0000AA);
const Color Color::Green        = FromHex(0xFF00AA00);
const Color Color::Cyan         = FromHex(0xFF00AAAA);
const Color Color::Red          = FromHex(0xFFAA0000);
const Color Color::Magenta      = FromHex(0xFFAA00AA);
const Color Color::Brown        = FromHex(0xFFAA5500);
const Color Color::LightGray    = FromHex(0xFFAAAAAA);
const Color Color::DarkGray     = FromHex(0xFF555555);
const Color Color::LightBlue    = FromHex(0xFF5555FF);
const Color Color::LightGreen   = FromHex(0xFF55FF55);
const Color Color::LightCyan    = FromHex(0xFF55FFFF);
const Color Color::LightRed     = FromHex(0xFFFF5555);
const Color Color::LightMagenta = FromHex(0xFFFF55FF);
const Color Color::Yellow       = FromHex(0xFFFFFF55);
const Color Color::White        = FromHex(0xFFFFFFFF);

const Color Color::Transparent  = FromHex(0x00000000);
// clang-format on

Color::Color() : R(0), G(0), B(0), A(255)
{
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a)
{
}

Color::Color(const char* value) : R(0), G(0), B(0), A(255)
{
    uint8_t r, g, b, a;

    if ((sscanf_s(value, "#%02hhx%02hhx%02hhx%02hhx", &a, &r, &g, &b) == 4) ||
        (sscanf_s(value, "rgba(%hhu,%hhu,%hhu,%hhu)", &r, &g, &b, &a) == 4))
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    else if ((sscanf_s(value, "#%02hhx%02hhx%02hhx", &r, &g, &b) == 3) ||
             (sscanf_s(value, "rgb(%hhu,%hhu,%hhu)", &r, &g, &b) == 3))
    {
        R = r;
        G = g;
        B = b;
        A = 255;
    }
}

Color Color::FromHex(uint32_t color)
{
    // clang-format off
    return {
        static_cast<uint8_t>((color >> 16) & 0xFF),
        static_cast<uint8_t>((color >> 8) & 0xFF),
        static_cast<uint8_t>((color & 0xFF)),
        static_cast<uint8_t>((color >> 24) & 0xFF)
    };
    // clang-format on
}

uint32_t Color::ToHex() const
{
    return (A << 24) | (R << 16) | (G << 8) | B;
}

std::string Color::ToString() const
{
    std::stringstream ss;
    ss << "#" << std::hex << A << R << G << B;
    return ss.str();
}

std::string Color::ToString(ColorFormats format) const
{
    std::stringstream ss;
    switch (format)
    {
    case ColorFormats::RGB:
        ss << "rgb(" << R << ", " << G << ", " << B << ")";
        break;
    case ColorFormats::RGBA:
        ss << "rgba(" << R << ", " << G << ", " << B << ", " << A << ")";
        break;
    case ColorFormats::HRGB:
        ss << "#" << std::hex << R << G << B;
        break;
    case ColorFormats::HARGB:
        ss << "#" << std::hex << A << R << G << B;
        break;
    }
    return ss.str();
}

bool Color::operator==(const Color& other) const
{
    return (R == other.R) && (G == other.G) && (B == other.B) && (A == other.A);
}

Color& Color::operator*=(float scalar)
{
    R = Math::Clamp<uint8_t>(static_cast<uint8_t>(R * scalar), 0, 255);
    G = Math::Clamp<uint8_t>(static_cast<uint8_t>(G * scalar), 0, 255);
    B = Math::Clamp<uint8_t>(static_cast<uint8_t>(B * scalar), 0, 255);
    A = Math::Clamp<uint8_t>(static_cast<uint8_t>(A * scalar), 0, 255);

    return *this;
}

Color& Color::operator/=(float scalar)
{
    return this->operator*=(1.0f / scalar);
}

Color Color::operator+(const Color& other) const
{
    return { static_cast<uint8_t>(Math::Clamp<int>(static_cast<int>(R) + static_cast<int>(other.R), 0, 255)),
             static_cast<uint8_t>(Math::Clamp<int>(static_cast<int>(G) + static_cast<int>(other.G), 0, 255)),
             static_cast<uint8_t>(Math::Clamp<int>(static_cast<int>(B) + static_cast<int>(other.B), 0, 255)), A };
}

Color Color::operator-(const Color& other) const
{
    return { static_cast<uint8_t>(Math::Clamp<int>(static_cast<int>(R) - static_cast<int>(other.R), 0, 255)),
             static_cast<uint8_t>(Math::Clamp<int>(static_cast<int>(G) - static_cast<int>(other.G), 0, 255)),
             static_cast<uint8_t>(Math::Clamp<int>(static_cast<int>(B) - static_cast<int>(other.B), 0, 255)), A };
}

Color operator*(const Color& color, float scalar)
{
    Color result(color);
    return result *= scalar;
}

Color operator*(float scalar, const Color& color)
{
    return color * scalar;
}

Color operator/(const Color& color, float scalar)
{
    Color result(color);
    return result /= scalar;
}

DGEX_END

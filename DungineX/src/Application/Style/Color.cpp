#include "DgeX/Application/Style/Color.h"

#include "DgeX/Utils/Math.h"

DGEX_BEGIN

static constexpr float Inverse255 = 1.0f / 255.0f;

// clang-format off
const Color Color::Black        = FromRGBA(0xFF000000);
const Color Color::Blue         = FromRGBA(0xFF0000AA);
const Color Color::Green        = FromRGBA(0xFF00AA00);
const Color Color::Cyan         = FromRGBA(0xFF00AAAA);
const Color Color::Red          = FromRGBA(0xFFAA0000);
const Color Color::Magenta      = FromRGBA(0xFFAA00AA);
const Color Color::Brown        = FromRGBA(0xFFAA5500);
const Color Color::LightGray    = FromRGBA(0xFFAAAAAA);
const Color Color::DarkGray     = FromRGBA(0xFF555555);
const Color Color::LightBlue    = FromRGBA(0xFF5555FF);
const Color Color::LightGreen   = FromRGBA(0xFF55FF55);
const Color Color::LightCyan    = FromRGBA(0xFF55FFFF);
const Color Color::LightRed     = FromRGBA(0xFFFF5555);
const Color Color::LightMagenta = FromRGBA(0xFFFF55FF);
const Color Color::Yellow       = FromRGBA(0xFFFFFF55);
const Color Color::White        = FromRGBA(0xFFFFFFFF);
// clang-format on

Color Color::FromRGBA(float r, float g, float b, float a)
{
    return { r * Inverse255, g * Inverse255, b * Inverse255, a * Inverse255 };
}

Color Color::FromRGBA(uint32_t color)
{
    float r = static_cast<float>((color >> 16) & 0xFF);
    float g = static_cast<float>((color >> 8) & 0xFF);
    float b = static_cast<float>(color & 0xFF);
    float a = static_cast<float>((color >> 24) & 0xFF);

    return { r * Inverse255, g * Inverse255, b * Inverse255, a * Inverse255 };
}

Color& Color::operator*=(float scalar)
{
    for (int i = 0; i < 4; i++)
    {
        RGBA[i] = Math::Clamp(RGBA[i] * scalar, 0.0f, 1.0f);
    }
    return *this;
}

// Warning: This function may cause a division by zero.
Color& Color::operator/=(float scalar)
{
    for (int i = 0; i < 4; i++)
    {
        RGBA[i] = Math::Clamp(RGBA[i] / scalar, 0.0f, 1.0f);
    }
    return *this;
}

Color operator*(const Color& color, float scalar)
{
    Color result = color;
    return result *= scalar;
}

Color operator*(float scalar, const Color& color)
{
    return color * scalar;
}

Color operator/(const Color& color, float scalar)
{
    Color result = color;
    return result /= scalar;
}

DGEX_END

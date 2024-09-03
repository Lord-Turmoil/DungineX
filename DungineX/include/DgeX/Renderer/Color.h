#pragma once

#include "DgeX/Core/Macros.h"

#include <glm/glm.hpp>

DGEX_BEGIN

struct Color
{
    static const Color Black;
    static const Color Blue;
    static const Color Green;
    static const Color Cyan;
    static const Color Red;
    static const Color Magenta;
    static const Color Brown;
    static const Color LightGray;
    static const Color DarkGray;
    static const Color LightBlue;
    static const Color LightGreen;
    static const Color LightCyan;
    static const Color LightRed;
    static const Color LightMagenta;
    static const Color Yellow;
    static const Color White;

    float R, G, B, A;

    Color() : R(0.0f), G(0.0f), B(0.0f), A(1.0f)
    {
    }

    Color(float r, float g, float b, float a = 1.0f) : R(r), G(g), B(b), A(a)
    {
    }

    uint32_t ToUInt32() const
    {
        uint32_t r = static_cast<uint32_t>(this->R * 255.0f) & 0xFF;
        uint32_t g = static_cast<uint32_t>(this->G * 255.0f) & 0xFF;
        uint32_t b = static_cast<uint32_t>(this->B * 255.0f) & 0xFF;
        uint32_t a = static_cast<uint32_t>(this->A * 255.0f) & 0xFF;
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    glm::vec4 ToVec4() const
    {
        return { this->R, this->G, this->B, this->A };
    }

    static Color From255(float r, float g, float b, float a = 255.0f)
    {
        return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
    }

    static Color FromUInt32(const uint32_t color)
    {
        float r = static_cast<float>((color >> 16) & 0xFF) / 255.0f;
        float g = static_cast<float>((color >> 8) & 0xFF) / 255.0f;
        float b = static_cast<float>(color & 0xFF) / 255.0f;
        float a = static_cast<float>((color >> 24) & 0xFF) / 255.0f;
        return { r, g, b, a };
    }
};

DGEX_END

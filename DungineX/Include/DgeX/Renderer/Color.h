#pragma once

#include "DgeX/dgexpch.h"

#include <glm/glm.hpp>

DGEX_BEGIN

struct Color
{
    float Red, Green, Blue, Alpha;

    Color() : Red(0.0f), Green(0.0f), Blue(0.0f), Alpha(1.0f)
    {
    }

    Color(float r, float g, float b, float a = 1.0f) : Red(r), Green(g), Blue(b), Alpha(a)
    {
    }

    uint32_t ToUInt32() const
    {
        uint32_t r = static_cast<uint32_t>(this->Red * 255.0f) & 0xFF;
        uint32_t g = static_cast<uint32_t>(this->Green * 255.0f) & 0xFF;
        uint32_t b = static_cast<uint32_t>(this->Blue * 255.0f) & 0xFF;
        uint32_t a = static_cast<uint32_t>(this->Alpha * 255.0f) & 0xFF;
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    glm::vec4 ToVec4() const
    {
        return { this->Red, this->Green, this->Blue, this->Alpha };
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

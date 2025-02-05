#pragma once

#include "DgeX/Utils/Easing.h"

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

    union
    {
        struct
        {
            float R, G, B, A;
        };

        float RGBA[4];
    };

    Color() : R(0.0f), G(0.0f), B(0.0f), A(1.0f)
    {
    }

    Color(float r, float g, float b, float a = 1.0f) : R(r), G(g), B(b), A(a)
    {
    }

    static Color FromRGBA(float r, float g, float b, float a = 1.0f);
    static Color FromRGBA(uint32_t color);

    uint32_t ToUInt32() const
    {
        uint32_t r = static_cast<uint32_t>(R * 0xFF) & 0xFF;
        uint32_t g = static_cast<uint32_t>(G * 0xFF) & 0xFF;
        uint32_t b = static_cast<uint32_t>(B * 0xFF) & 0xFF;
        uint32_t a = static_cast<uint32_t>(A * 0xFF) & 0xFF;
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    glm::vec4 ToVec4() const
    {
        return { R, G, B, A };
    }

    Color& operator*=(float scalar);
    Color& operator/=(float scalar);

    friend Color operator*(const Color& color, float scalar);
    friend Color operator*(float scalar, const Color& color);
    friend Color operator/(const Color& color, float scalar);
};

namespace Utils::Easing
{

// Specialization for Color
template <typename S> constexpr Color Interpolate(Color initial, Color target, S step, EasingFn<S> fn)
{
    auto factor = fn(step);

    Color blend;
    for (int i = 0; i < 4; ++i)
    {
        blend.RGBA[i] = initial.RGBA[i] + (target.RGBA[i] - initial.RGBA[i]) * factor;
        blend.RGBA[i] = Math::Clamp(blend.RGBA[i], 0.0f, 255.0f);
    }

    return blend;
}

} // namespace Utils::Easing

DGEX_END

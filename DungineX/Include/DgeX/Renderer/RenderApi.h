#pragma once

#include "DgeX/dgexpch.h"

#include "DgeX/Renderer/VertexArray.h"

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

namespace RenderApi
{

/**
 * @brief Initialize the renderer backend API.
 */
void Init();

/**
 * @brief Set viewport.
 * @param x origin x
 * @param y origin y
 * @param width width
 * @param height height
 */
void SetViewport(int x, int y, int width, int height);
/**
 * @brief Set clear color, i.e. background color.
 * @param color clear color
 */
void SetClearColor(const Color& color);
/**
 * @brief Clear the device, i.e. clear the screen.
 */
void ClearDevice();

/**
 * @brief Set color.
 * @param color the following color to be used
 */
void SetColor(const Color& color);

/**
 * @brief Set line width.
 * @param width line width
 */
void SetLineWidth(float width);

/**
 * @brief Draw index triangles
 * @param vertexArray vertices to draw
 * @param indexCount how many vertices to draw, 0 to draw all
 */
static void DrawIndexed(const Ref<VertexArray>& vertexArray, int indexCount = 0);

/**
 * @brief Draw lines
 * @param vertexArray vertices to draw
 * @param vertexCount how many vertices to draw, 0 to draw all
 */
static void DrawLines(const Ref<VertexArray>& vertexArray, int vertexCount = 0);

} // namespace RenderApi

DGEX_END

#pragma once

#include "DgeX/dgexpch.h"

#include "DgeX/Renderer/Color.h"
#include "DgeX/Renderer/VertexArray.h"

DGEX_BEGIN

namespace RenderCommand
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
 * @brief Set line width.
 * @param width line width
 */
void SetLineWidth(float width);

/**
 * @brief Draw index triangles
 * @param vertexArray vertices to draw
 * @param indexCount how many vertices to draw, 0 to draw all
 */
void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);

/**
 * @brief Draw lines
 * @param vertexArray vertices to draw
 * @param vertexCount how many vertices to draw, 0 to draw all
 */
void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0);

} // namespace RenderCommand

DGEX_END
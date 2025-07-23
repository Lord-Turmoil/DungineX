/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderApi.h                               *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 19, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Render APIs.                                                               *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Error.h"
#include "DgeX/Renderer/Color.h"
#include "DgeX/Utils/Types.h"

DGEX_BEGIN

class Font;
class Renderer;
class Texture;
class TextureRenderCommandBuilder;

// ============================================================================
// Render & Target Settings
// ----------------------------------------------------------------------------

/**
 * @brief Initialize render API.
 *
 * @return 0 on success, otherwise failure.
 */
dgex_error_t InitRenderApi();

/**
 * @brief Destroy render API.
 */
void DestroyRenderApi();

/**
 * @brief Set the current renderer.
 *
 * Use nullptr for default renderer.
 *
 * @param renderer The renderer to set.
 */
DGEX_API void SetCurrentRenderer(const Ref<Renderer>& renderer = nullptr);

/**
 * @brief Get the current renderer.
 *
 * @return The current renderer.
 */
DGEX_API Ref<Renderer> GetCurrentRenderer();

class RendererGuard
{
public:
    DGEX_API explicit RendererGuard(const Ref<Renderer>& renderer);
    DGEX_API RendererGuard(const RendererGuard& other) = delete;
    DGEX_API RendererGuard(RendererGuard&& other) noexcept = delete;
    DGEX_API RendererGuard& operator=(const RendererGuard& other) = delete;
    DGEX_API RendererGuard& operator=(RendererGuard&& other) noexcept = delete;

    DGEX_API ~RendererGuard();

private:
    Ref<Renderer> _lastRenderer;
};

/**
 * @brief Use a renderer in the current scope.
 *
 * Will automatically reset to the last render when leave the scope.
 *
 * @param renderer The renderer to use.
 */
#define USE_RENDERER(renderer) RendererGuard __dgex_renderer_guard((renderer))

/**
 * @brief Set the current render target for the current renderer.
 *
 * Use nullptr for screen.
 *
 * @param texture The render target texture.
 */
DGEX_API void SetCurrentRenderTarget(const Ref<Texture>& texture = nullptr);

/**
 * @brief Get the current render target.
 *
 * nullptr is for screen.
 *
 * @return The current render target.
 */
DGEX_API Ref<Texture> GetCurrentRenderTarget();

class RenderTargetGuard
{
public:
    DGEX_API explicit RenderTargetGuard(const Ref<Texture>& texture);
    DGEX_API RenderTargetGuard(const RenderTargetGuard& other) = delete;
    DGEX_API RenderTargetGuard(RenderTargetGuard&& other) noexcept = delete;
    DGEX_API RenderTargetGuard& operator=(const RenderTargetGuard& other) = delete;
    DGEX_API RenderTargetGuard& operator=(RenderTargetGuard&& other) noexcept = delete;

    DGEX_API ~RenderTargetGuard();

private:
    Ref<Texture> _lastRenderTarget;
};

/**
 * @brief Use a render target in the current scope.
 *
 * Will automatically reset to the last render target when leave the scope.
 *
 * @param texture The texture to use as the render target.
 */
#define USE_RENDER_TARGET(texture) RenderTargetGuard __dgex_render_target_guard((texture))

// ============================================================================
// Render Property Settings
// ----------------------------------------------------------------------------

/**
 * @brief Set clear color.
 *
 * This color will be used to clear the screen or canvas.
 *
 * @param color Clear color.
 */
DGEX_API void SetClearColor(Color color);

/**
 * @brief Get the current clear color.
 *
 * @return Current clear color.
 */
DGEX_API Color GetClearColor();

/**
 * @brief Set current line color.
 *
 * @param color Line color.
 */
DGEX_API void SetLineColor(Color color);

/**
 * @brief Get the current line color.
 *
 * @return Current line color.
 */
DGEX_API Color GetLineColor();

/**
 * @brief Set current fill color.
 */
DGEX_API void SetFillColor(Color color);

/**
 * @brief Get the current fill color.
 *
 * @return Current fill color.
 */
DGEX_API Color GetFillColor();

/**
 * @brief Set current font.
 *
 * @param font Current font.
 */
DGEX_API void SetFont(const Ref<Font>& font);

/**
 * @brief Get the current font.
 *
 * @return Current font.
 */
DGEX_API Ref<Font> GetFont();

/**
 * @brief Set the current font color.
 *
 * @param color Font color to set.
 */
DGEX_API void SetFontColor(Color color);

/**
 * @brief Get the current font color.
 *
 * @return Current font color.
 */
DGEX_API Color GetFontColor();

/**
 * @brief Set the font size.
 * @param pointSize Font size in points (pixels).
 */
DGEX_API void SetFontSize(float pointSize);

/**
 * @brief Get the current font size.
 *
 * @return Current font size in points (pixels).
 */
DGEX_API float GetFontSize();

// ============================================================================
// Device Render API
// ----------------------------------------------------------------------------

/**
 * @brief Clear current render target.
 *
 * This will be applied before any render command if using ordered renderer.
 */
DGEX_API void ClearDevice();

/**
 * @brief Update screen.
 *
 * This is only used to update the screen, which by default use back buffer.
 */
DGEX_API void FlushDevice();

// ============================================================================
// Primitives Render API
// ----------------------------------------------------------------------------

/**
 * @brief Draw a point.
 *
 * Note that point also uses line color.
 *
 * @param x The x coordinate of the point.
 * @param y The y coordinate of the point.
 * @param z The z index of the point for sorting.
 */
DGEX_API void DrawPoint(int x, int y, int z = 0);

/**
 * @brief Draw a line.
 * @param x1 The x coordinate of the start point.
 * @param y1 The y coordinate of the start point.
 * @param x2 The x coordinate of the end point.
 * @param y2 The y coordinate of the end point.
 * @param z The z index of the line for sorting.
 */
DGEX_API void DrawLine(int x1, int y1, int x2, int y2, int z = 0);

/**
 * @brief Draw a rectangle outline without filling.
 *
 * @param x The x coordinate of the top-left corner of the rectangle.
 * @param y The y coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param z The z index for sorting.
 */
DGEX_API void DrawRect(int x, int y, int width, int height, int z = 0);

/**
 * @brief Draw a rectangle outline without filling.
 *
 * @param rect The rectangle to draw.
 * @param z The z index for sorting.
 */
DGEX_API void DrawRect(const Rect& rect, int z = 0);

/**
 * @brief Draw a filled rectangle without outline.
 *
 * @param x The x coordinate of the top-left corner of the rectangle.
 * @param y The y coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param z The z index for sorting.
 */
DGEX_API void DrawFilledRect(int x, int y, int width, int height, int z = 0);

/**
 * @brief Draw a filled rectangle without outline.
 *
 * @param rect The rectangle to draw.
 * @param z The z index for sorting.
 */
DGEX_API void DrawFilledRect(const Rect& rect, int z = 0);

// ============================================================================
// Texture Render API
// ----------------------------------------------------------------------------

/**
 * @brief Draw a texture.
 *
 * @param texture The texture to draw.
 * @param x The x coordinate of the top-left corner of the texture.
 * @param y The y coordinate of the top-left corner of the texture.
 * @param z The z index for sorting.
 */
DGEX_API void DrawTexture(const Ref<Texture>& texture, int x, int y, int z = 0);

/**
 * @brief Draw a texture with additional styles.
 *
 * By default, the transformation is applied around the center of the texture.
 * If you want to apply the transformation around a specific point, use the overloaded version
 * with `TextureAnchor`.
 *
 *
 * @param texture The texture to draw.
 * @param style The style of the texture.
 * @param x The x coordinate of the top-left corner of the texture.
 * @param y The y coordinate of the top-left corner of the texture.
 * @param z The z index for sorting.
 * @return Fluent-API style actions.
 */
DGEX_API void DrawTexture(const Ref<Texture>& texture, const TextureStyle& style, int x, int y, int z = 0);

/**
 * @brief
 * @param texture The texture to draw.
 * @param style The style of the texture.
 * @param anchor The anchor to apply scale and rotation.
 * @param x The x coordinate of the top-left corner of the texture.
 * @param y The y coordinate of the top-left corner of the texture.
 * @param z The z index for sorting.
 */
DGEX_API void DrawTexture(const Ref<Texture>& texture, const TextureStyle& style, const TextureAnchor& anchor, int x,
                          int y, int z = 0);

// ============================================================================
// Text Render API
// ----------------------------------------------------------------------------

/**
 * @brief Render text according to a point.
 *
 * @warning The text should be valid until the next rendering batch.
 *
 * @param text Text to render.
 * @param x The x coordinate to render the text.
 * @param y The y coordinate to render the text.
 * @param flags Controls how to render the text.
 */
DGEX_API void DrawText(const char* text, int x, int y, TextFlags flags);

/**
 * @brief Render text in a rectangle area.
 *
 * @warning The text should be valid until the next rendering batch.
 *
 * @param text Text to render.
 * @param rect The text area.
 * @param flags Controls how to render the text.
 */
DGEX_API void DrawTextArea(const char* text, const Rect& rect, TextFlags flags);

DGEX_END

/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderApi.cpp                             *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 3, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Render APIs.                                                               *
 ******************************************************************************/

#include "DgeX/Renderer/RenderApi.h"

#include "Renderer/RenderApiImpl.h"
#include "Renderer/RenderCommandImpl.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Renderer/Font.h"
#include "DgeX/Renderer/Texture.h"
#include "DgeX/Utils/Assert.h"

#include <SDL3/SDL.h>
#include <SDL_FontCache/SDL_FontCache.h>

#include <climits>

DGEX_BEGIN

struct RenderApiContext
{
    Color ClearColor;
    Color LineColor;
    Color FillColor;

    Color FontColor;
    Ref<Font> Font;
    float FontSize;
};

static Ref<Renderer> sActiveRenderer = nullptr;
static Ref<Texture> sActiveRenderTarget = nullptr;

static RenderApiContext sContext;

dgex_error_t InitRenderApi()
{
    // Initialize context.
    sContext.ClearColor = Color::Black;
    sContext.LineColor = Color::White;
    sContext.FillColor = Color::White;
    sContext.FontColor = Color::White;
    sContext.Font = nullptr;
    sContext.FontSize = 16.0f;

    Ref<Font> font = LoadFont("C:/Windows/Fonts/Arial.ttf");
    if (!font)
    {
        DGEX_CORE_ERROR("Failed to load default system font");
        return DGEX_ERROR_RENDERER_API_INIT;
    }
    SetFont(font);

    DGEX_CORE_DEBUG("Render API initialized");

    return DGEX_SUCCESS;
}

void DestroyRenderApi()
{
    DGEX_CORE_DEBUG("Render API destroyed");
}

void SetCurrentRenderer(const Ref<Renderer>& renderer)
{
    sActiveRenderer = renderer;
}

Ref<Renderer> GetCurrentRenderer()
{
    return sActiveRenderer;
}

RendererGuard::RendererGuard(const Ref<Renderer>& renderer) : _lastRenderer(GetCurrentRenderer())
{
    SetCurrentRenderer(renderer);
}

RendererGuard::~RendererGuard()
{
    SetCurrentRenderer(_lastRenderer);
}

// Reference: https://wiki.libsdl.org/SDL3/SDL_SetRenderTarget
void SetCurrentRenderTarget(const Ref<Texture>& texture)
{
    sActiveRenderTarget = texture;

    SDL_Texture* target = sActiveRenderTarget ? sActiveRenderTarget->GetNativeTexture() : nullptr;

    SDL_SetRenderTarget(GetNativeRenderer(), target);
}

Ref<Texture> GetCurrentRenderTarget()
{
    return sActiveRenderTarget;
}

RenderTargetGuard::RenderTargetGuard(const Ref<Texture>& texture) : _lastRenderTarget(GetCurrentRenderTarget())
{
    SetCurrentRenderTarget(texture);
}

RenderTargetGuard::~RenderTargetGuard()
{
    SetCurrentRenderTarget(_lastRenderTarget);
}

// ============================================================================
// Render Property Settings
// ----------------------------------------------------------------------------
// Reference: https://wiki.libsdl.org/SDL3/SDL_SetRenderDrawColor
// ----------------------------------------------------------------------------

void SetClearColor(Color color)
{
    sContext.ClearColor = color;
}

Color GetClearColor()
{
    return sContext.ClearColor;
}

void SetLineColor(Color color)
{
    sContext.LineColor = color;
}

Color GetLineColor()
{
    return sContext.LineColor;
}

void SetFillColor(Color color)
{
    sContext.FillColor = color;
}

Color GetFillColor()
{
    return sContext.FillColor;
}

void SetFont(const Ref<Font>& font)
{
    if (!font)
    {
        DGEX_CORE_WARN("Cannot set empty font");
        return;
    }
    sContext.Font = font;
}

Ref<Font> GetFont()
{
    return sContext.Font;
}

void SetFontColor(Color color)
{
    sContext.FontColor = color;
}

Color GetFontColor()
{
    return sContext.FontColor;
}

void SetFontSize(float pointSize)
{
    sContext.FontSize = pointSize;
}

float GetFontSize()
{
    return sContext.FontSize;
}

// ============================================================================
// Device Render API
// ----------------------------------------------------------------------------
// References:
// - https://wiki.libsdl.org/SDL2/SDL_RenderClear
// - https://wiki.libsdl.org/SDL3/SDL_RenderPresent
// ----------------------------------------------------------------------------

void ClearDevice()
{
    SDL_Renderer* renderer = GetNativeRenderer();
    SetDrawColorImpl(renderer, sContext.ClearColor);
    SDL_RenderClear(renderer);
}

void FlushDevice()
{
    SDL_RenderPresent(GetNativeRenderer());
}

// ============================================================================
// Primitives Render API
// ----------------------------------------------------------------------------
// References:
// - https://wiki.libsdl.org/SDL3/SDL_RenderPoint
// - https://wiki.libsdl.org/SDL3/SDL_RenderLine
// - https://wiki.libsdl.org/SDL3/SDL_RenderRect
// ----------------------------------------------------------------------------

void DrawPoint(int x, int y, int z)
{
    if (sActiveRenderer)
    {
        Color color = sContext.LineColor;
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [x, y, color](SDL_Renderer* renderer) { DrawPointImpl(renderer, x, y, color); }, z));
    }
    else
    {
        DrawPointImpl(GetNativeRenderer(), x, y, sContext.LineColor);
    }
}

void DrawLine(int x1, int y1, int x2, int y2, int z)
{
    if (sActiveRenderer)
    {
        Color color = sContext.LineColor;
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [x1, y1, x2, y2, color](SDL_Renderer* renderer) { DrawLineImpl(renderer, x1, y1, x2, y2, color); }, z));
    }
    else
    {
        DrawLineImpl(GetNativeRenderer(), x1, y1, x2, y2, sContext.LineColor);
    }
}

void DrawRect(int x, int y, int width, int height, int z)
{
    DrawRect(Rect(x, y, width, height), z);
}

void DrawRect(const Rect& rect, int z)
{
    if (sActiveRenderer)
    {
        Color color = sContext.LineColor;
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [rect, color](SDL_Renderer* renderer) { DrawRectImpl(renderer, rect, color); }, z));
    }
    else
    {
        DrawRectImpl(GetNativeRenderer(), rect, sContext.LineColor);
    }
}

void DrawFilledRect(int x, int y, int width, int height, int z)
{
    DrawFilledRect(Rect(x, y, width, height), z);
}

void DrawFilledRect(const Rect& rect, int z)
{
    if (sActiveRenderer)
    {
        Color color = sContext.FillColor;
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [rect, color](SDL_Renderer* renderer) { DrawFilledRectImpl(renderer, rect, color); }, z));
    }
    else
    {
        DrawFilledRectImpl(GetNativeRenderer(), rect, sContext.FillColor);
    }
}

// ============================================================================
// Texture Render API
// ----------------------------------------------------------------------------

// Simple texture rendering can be implemented simply.
void DrawTexture(const Ref<Texture>& texture, int x, int y, int z)
{
    if (sActiveRenderer)
    {
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [texture, x, y](SDL_Renderer* renderer) { DrawTextureImpl(renderer, texture->GetNativeTexture(), x, y); },
            z));
    }
    else
    {
        DrawTextureImpl(GetNativeRenderer(), texture->GetNativeTexture(), x, y);
    }
}

void DrawTexture(const Ref<Texture>& texture, const TextureStyle& style, int x, int y, int z)
{
    if (sActiveRenderer)
    {
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [texture, style, x, y](SDL_Renderer* renderer) {
                DrawTextureImpl(renderer, texture->GetNativeTexture(), style, x, y);
            },
            z));
    }
    else
    {
        DrawTextureImpl(GetNativeRenderer(), texture->GetNativeTexture(), style, x, y);
    }
}

void DrawTexture(const Ref<Texture>& texture, const TextureStyle& style, const TextureAnchor& anchor, int x, int y,
                 int z)
{
    if (sActiveRenderer)
    {
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [texture, style, anchor, x, y](SDL_Renderer* renderer) {
                DrawTextureImpl(renderer, texture->GetNativeTexture(), style, anchor, x, y);
            },
            z));
    }
    else
    {
        DrawTextureImpl(GetNativeRenderer(), texture->GetNativeTexture(), style, anchor, x, y);
    }
}

void DrawText(const char* text, int x, int y, TextFlags flags)
{
    if (!sContext.Font)
    {
        DGEX_CORE_WARN("No font specified");
        return;
    }

    auto font = static_cast<FC_Font*>(sContext.Font->GetImpl());
    float scale = GetFontScale(sContext.FontSize);

    if (sActiveRenderer)
    {
        Color color = sContext.FontColor;
        sActiveRenderer->Submit(
            NativeRenderCommand::Create([font, text, x, y, color, scale, flags](SDL_Renderer* renderer) {
                DrawTextImpl(renderer, font, text, x, y, color, scale, flags);
            }));
    }
    else
    {
        DrawTextImpl(GetNativeRenderer(), font, text, x, y, sContext.FontColor, scale, flags);
    }
}

void DrawTextArea(const char* text, const Rect& rect, TextFlags flags)
{
    if (!sContext.Font)
    {
        DGEX_CORE_WARN("No font specified");
        return;
    }

    auto font = static_cast<FC_Font*>(sContext.Font->GetImpl());
    float scale = GetFontScale(sContext.FontSize);
    FC_Rect fcRect{ rect.X, rect.Y, rect.Width, rect.Height };

    if (sActiveRenderer)
    {
        Color color = sContext.FontColor;
        sActiveRenderer->Submit(
            NativeRenderCommand::Create([font, text, fcRect, color, scale, flags](SDL_Renderer* renderer) {
                DrawTextAreaImpl(renderer, font, text, fcRect, color, scale, flags);
            }));
    }
    else
    {
        DrawTextAreaImpl(GetNativeRenderer(), font, text, fcRect, sContext.FontColor, scale, flags);
    }
}

DGEX_END

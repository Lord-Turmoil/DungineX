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

#include "Device/Graphics/RenderCommandImpl.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Renderer/Texture.h"
#include "DgeX/Utils/Assert.h"

#include <SDL3/SDL.h>

DGEX_BEGIN

struct RenderApiContext
{
    Color ClearColor = Color::Black;
    Color LineColor = Color::White;
    Color FillColor = Color::White;
};

static Ref<Renderer> sActiveRenderer = nullptr;
static Ref<Texture> sActiveRenderTarget = nullptr;

static RenderApiContext sContext;

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

    // Fun fact, these two renderers are actually the same.
    SDL_Renderer* renderer = sActiveRenderer ? sActiveRenderer->GetNativeRenderer() : GetNativeRenderer();
    SDL_Texture* target = sActiveRenderTarget ? sActiveRenderTarget->GetNativeTexture() : nullptr;

    SDL_SetRenderTarget(renderer, target);
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

/**
 * @brief Set SDL draw color.
 *
 * @param renderer Specified renderer.
 * @param color Draw color.
 */
static void SetDrawColor(SDL_Renderer* renderer, const Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
}

void SetClearColor(Color color)
{
    sContext.ClearColor = color;
}

Color GetClearColor()
{
    return sContext.ClearColor;
}

ClearColorGuard::ClearColorGuard(Color color) : _lastClearColor(GetClearColor())
{
    SetClearColor(color);
}

ClearColorGuard::~ClearColorGuard()
{
    SetClearColor(_lastClearColor);
}

void SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SetClearColor(Color(r, g, b, a));
}

void SetLineColor(const Color& color)
{
    sContext.LineColor = color;
}

void SetLineColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SetLineColor(Color(r, g, b, a));
}

Color GetLineColor()
{
    return sContext.LineColor;
}

LineColorGuard::LineColorGuard(Color color) : _lastLineColor(GetLineColor())
{
    SetLineColor(color);
}

LineColorGuard::~LineColorGuard()
{
    SetLineColor(_lastLineColor);
}

void SetFillColor(const Color& color)
{
    sContext.FillColor = color;
}

void SetFillColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SetFillColor(Color(r, g, b, a));
}

Color GetFillColor()
{
    return sContext.FillColor;
}

FillColorGuard::FillColorGuard(Color color) : _lastFillColor(GetFillColor())
{
    SetFillColor(color);
}

FillColorGuard::~FillColorGuard()
{
    SetFillColor(_lastFillColor);
}

// ============================================================================
// Device Render API
// ----------------------------------------------------------------------------
// References:
// - https://wiki.libsdl.org/SDL2/SDL_RenderClear
// - https://wiki.libsdl.org/SDL3/SDL_RenderPresent
// ----------------------------------------------------------------------------

static void ClearDeviceImpl(SDL_Renderer* renderer, Color color)
{
    SetDrawColor(renderer, color);
    SDL_RenderClear(renderer);
}

void ClearDevice()
{
    if (sActiveRenderer)
    {
        Color color = sContext.ClearColor;
        sActiveRenderer->SubmitImmediate(
            NativeRenderCommand::Create([color](SDL_Renderer* renderer) { ClearDeviceImpl(renderer, color); }));
    }
    else
    {
        ClearDeviceImpl(GetNativeRenderer(), sContext.ClearColor);
    }
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

static void DrawPointImpl(SDL_Renderer* renderer, int x, int y, Color color)
{
    SetDrawColor(renderer, color);
    SDL_RenderPoint(renderer, static_cast<float>(x), static_cast<float>(y));
}

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

static void DrawLineImpl(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Color color)
{
    SetDrawColor(renderer, color);
    SDL_RenderLine(renderer, static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2),
                   static_cast<float>(y2));
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

static void DrawRectImpl(SDL_Renderer* renderer, int x, int y, int width, int height, Color color)
{
    SetDrawColor(renderer, color);
    SDL_FRect rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width),
                    static_cast<float>(height) };
    SDL_RenderRect(renderer, &rect);
}

void DrawRect(int x, int y, int width, int height, int z)
{
    if (sActiveRenderer)
    {
        Color color = sContext.LineColor;
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [x, y, width, height, color](SDL_Renderer* renderer) {
                DrawRectImpl(renderer, x, y, width, height, color);
            },
            z));
    }
    else
    {
        DrawRectImpl(GetNativeRenderer(), x, y, width, height, sContext.LineColor);
    }
}

static void DrawFilledRectImpl(SDL_Renderer* renderer, int x, int y, int width, int height, Color color)
{
    SetDrawColor(renderer, color);
    SDL_FRect rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width),
                    static_cast<float>(height) };
    SDL_RenderFillRect(renderer, &rect);
}

void DrawFilledRect(int x, int y, int width, int height, int z)
{
    if (sActiveRenderer)
    {
        Color color = sContext.FillColor;
        sActiveRenderer->Submit(NativeRenderCommand::Create(
            [x, y, width, height, color](SDL_Renderer* renderer) {
                DrawFilledRectImpl(renderer, x, y, width, height, color);
            },
            z));
    }
    else
    {
        DrawFilledRectImpl(GetNativeRenderer(), x, y, width, height, sContext.FillColor);
    }
}

// ============================================================================
// Texture Render API
// ----------------------------------------------------------------------------

static void DrawTextureImpl(const Ref<TextureRenderCommand>& command)
{
    if (sActiveRenderer)
    {
        sActiveRenderer->Submit(command);
    }
    else
    {
        command->Apply(GetNativeRenderer());
    }
}

void DrawTexture(const Ref<Texture>& texture, int x, int y, int z)
{
    DrawTextureImpl(TextureRenderCommandBuilder(texture->GetNativeTexture()).SetPosition(x, y, z).Create());
}

DrawTextureClause::DrawTextureClause(const Ref<Texture>& texture, int x, int y, int z)
    : _builder(CreateRef<TextureRenderCommandBuilder>(texture->GetNativeTexture()))
{
    _builder->SetPosition(x, y, z);
}

DrawTextureClause DrawTextureClause::Scale(float scale)
{
    _builder->SetScale(scale);
    return *this;
}

DrawTextureClause DrawTextureClause::Rotate(float degree)
{
    _builder->SetRotation(degree);
    return *this;
}

DrawTextureClause DrawTextureClause::Anchor(int x, int y)
{
    _builder->SetAnchor(x, y);
    return *this;
}

DrawTextureClause DrawTextureClause::Alpha(uint8_t alpha)
{
    _builder->SetAlpha(alpha);
    return *this;
}

void DrawTextureClause::Submit()
{
    DrawTextureImpl(_builder->Create());
}

DrawTextureClause DrawTextureBegin(const Ref<Texture>& texture, int x, int y, int z)
{
    return { texture, x, y, z };
}

DGEX_END

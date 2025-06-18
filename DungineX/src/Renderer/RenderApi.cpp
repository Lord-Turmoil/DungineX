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
#include "DgeX/Renderer/Font.h"
#include "DgeX/Renderer/Texture.h"
#include "DgeX/Utils/Assert.h"

#include <SDL3/SDL.h>
#include <SDL_FontCache/SDL_FontCache.h>

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

void SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SetClearColor(Color(r, g, b, a));
}

void SetLineColor(Color color)
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

void SetFillColor(Color color)
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

void SetFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SetFontColor(Color(r, g, b, a));
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

void DrawRect(const Rect& rect, int z)
{
    DrawRect(rect.X, rect.Y, rect.Width, rect.Height, z);
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

void DrawFilledRect(const Rect& rect, int z)
{
    DrawFilledRect(rect.X, rect.Y, rect.Width, rect.Height, z);
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

static void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y)
{
    SDL_PropertiesID props = SDL_GetTextureProperties(texture);
    float width = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    float height = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));
    SDL_FRect rect{ static_cast<float>(x), static_cast<float>(y), width, height };
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

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

static void DrawTextImpl(SDL_Renderer* renderer, FC_Font* font, const char* text, int x, int y, Color color,
                         float scale, TextFlags flags)
{
    FC_Effect effect;

    if (flags & DGEX_TextAlignRight)
    {
        effect.alignment = FC_ALIGN_RIGHT;
    }
    else if (flags & DGEX_TextAlignCenter)
    {
        effect.alignment = FC_ALIGN_CENTER;
    }
    else
    {
        effect.alignment = FC_ALIGN_LEFT;
    }

    effect.scale = FC_MakeScale(scale, scale);
    effect.color = FC_MakeColor(color.R, color.G, color.B, color.A);

    FC_DrawEffect(font, renderer, static_cast<float>(x), static_cast<float>(y), effect, text);
}

static void DrawTextAreaImpl(SDL_Renderer* renderer, FC_Font* font, const char* text, FC_Rect rect, Color color,
                             float scale, TextFlags flags)
{
    FC_Effect effect;

    if (flags & DGEX_TextAlignRight)
    {
        effect.alignment = FC_ALIGN_RIGHT;
    }
    else if (flags & DGEX_TextAlignCenter)
    {
        effect.alignment = FC_ALIGN_CENTER;
    }
    else
    {
        effect.alignment = FC_ALIGN_LEFT;
    }

    effect.scale = FC_MakeScale(scale, scale);
    effect.color = FC_MakeColor(color.R, color.G, color.B, color.A);

    if (flags & DGEX_TextOverflow)
    {
        FC_DrawColumnEffect(font, renderer, static_cast<float>(rect.x), static_cast<float>(rect.y),
                            static_cast<Uint16>(rect.w), effect, text);
    }
    else
    {
        FC_DrawBoxEffect(font, renderer, rect, effect, text);
    }
}

void DrawText(const char* text, int x, int y, TextFlags flags)
{
    if (!sContext.Font)
    {
        DGEX_CORE_WARN("No font specified");
        return;
    }

    FC_Font* font = static_cast<FC_Font*>(sContext.Font->GetImpl());
    SDL_Renderer* renderer = sActiveRenderer ? sActiveRenderer->GetNativeRenderer() : GetNativeRenderer();

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
        DrawTextImpl(renderer, font, text, x, y, sContext.FontColor, scale, flags);
    }
}

void DrawTextArea(const char* text, int x, int y, int width, int height, TextFlags flags)
{
    if (!sContext.Font)
    {
        DGEX_CORE_WARN("No font specified");
        return;
    }

    FC_Font* font = static_cast<FC_Font*>(sContext.Font->GetImpl());
    float scale = GetFontScale(sContext.FontSize);
    FC_Rect rect{ x, y, width, height };

    if (sActiveRenderer)
    {
        Color color = sContext.FontColor;
        sActiveRenderer->Submit(
            NativeRenderCommand::Create([font, text, rect, color, scale, flags](SDL_Renderer* renderer) {
                DrawTextAreaImpl(renderer, font, text, rect, color, scale, flags);
            }));
    }
    else
    {
        DrawTextAreaImpl(GetNativeRenderer(), font, text, rect, sContext.FontColor, scale, flags);
    }
}

void DrawTextArea(const char* text, const Rect& rect, TextFlags flags)
{
    DrawTextArea(text, rect.X, rect.Y, rect.Width, rect.Height, flags);
}

DGEX_END

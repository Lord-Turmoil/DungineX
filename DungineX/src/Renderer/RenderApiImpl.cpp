/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderApiImpl.cpp                         *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : July 23, 2025                             *
 *                                                                            *
 *                    Last Update : July 23, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Render API implementation.                                                 *
 ******************************************************************************/

#include "Renderer/RenderApiImpl.h"

DGEX_BEGIN

void SetDrawColorImpl(SDL_Renderer* renderer, const Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
}

void DrawPointImpl(SDL_Renderer* renderer, int x, int y, Color color)
{
    SetDrawColorImpl(renderer, color);
    SDL_RenderPoint(renderer, static_cast<float>(x), static_cast<float>(y));
}

void DrawLineImpl(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Color color)
{
    SetDrawColorImpl(renderer, color);
    SDL_RenderLine(renderer, static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2),
                   static_cast<float>(y2));
}

void DrawRectImpl(SDL_Renderer* renderer, const Rect& rect, Color color)
{
    SetDrawColorImpl(renderer, color);
    SDL_FRect sdlRect{ static_cast<float>(rect.X), static_cast<float>(rect.Y), static_cast<float>(rect.Width),
                       static_cast<float>(rect.Height) };
    SDL_RenderRect(renderer, &sdlRect);
}

void DrawFilledRectImpl(SDL_Renderer* renderer, const Rect& rect, Color color)
{
    SetDrawColorImpl(renderer, color);
    SDL_FRect sdlRect{ static_cast<float>(rect.X), static_cast<float>(rect.Y), static_cast<float>(rect.Width),
                       static_cast<float>(rect.Height) };
    SDL_RenderFillRect(renderer, &sdlRect);
}

void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y)
{
    SDL_PropertiesID props = SDL_GetTextureProperties(texture);
    float width = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    float height = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));
    SDL_FRect rect{ static_cast<float>(x), static_cast<float>(y), width, height };
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

static void GetDrawTextureProps(SDL_Texture* texture, const TextureStyle& style, int x, int y, SDL_FRect* rect,
                                float* degree, SDL_FlipMode* flip)
{
    SDL_PropertiesID props = SDL_GetTextureProperties(texture);

    float width = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    float height = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));

    // Scale the source rectangle around the center of the texture.
    float xOffset = -width * (style.Scale - 1.0f) * 0.5f;
    float yOffset = -height * (style.Scale - 1.0f) * 0.5f;
    *rect = { static_cast<float>(x) + xOffset, static_cast<float>(y) + yOffset, width * style.Scale,
              height * style.Scale };

    // Set additional alpha.
    SDL_SetTextureAlphaMod(texture, style.Alpha);

    // Rotate the texture around the center.
    *flip = SDL_FLIP_NONE;
    *degree = style.Degree;
    if (style.FlipX && style.FlipY)
    {
        // Flip horizontally and vertically is equivalent to rotate 180deg.
        *degree += 180.0f;
    }
    else if (style.FlipX)
    {
        *flip = SDL_FLIP_HORIZONTAL;
    }
    else if (style.FlipY)
    {
        *flip = SDL_FLIP_VERTICAL;
    }
}

void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, const TextureStyle& style, int x, int y)
{
    float degree;
    SDL_FRect rect;
    SDL_FlipMode flip;

    GetDrawTextureProps(texture, style, x, y, &rect, &degree, &flip);
    SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, degree, nullptr, flip);
}

void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, const TextureStyle& style,
                     const TextureAnchor& anchor, int x, int y)
{
    float degree;
    SDL_FRect rect;
    SDL_FlipMode flip;
    SDL_FPoint fAnchor = { static_cast<float>(anchor.X), static_cast<float>(anchor.Y) };

    GetDrawTextureProps(texture, style, x, y, &rect, &degree, &flip);
    SDL_RenderTextureRotated(renderer, texture, nullptr, &rect, degree, &fAnchor, flip);
}

static void GetDrawTextProps(Color color, float scale, TextFlags flags, FC_Effect* effect)
{
    effect->scale = FC_MakeScale(scale, scale);
    effect->color = FC_MakeColor(color.R, color.G, color.B, color.A);

    if (flags & L(TextFlag::AlignRight))
    {
        effect->alignment = FC_ALIGN_RIGHT;
    }
    else if (flags & L(TextFlag::AlignCenter))
    {
        effect->alignment = FC_ALIGN_CENTER;
    }
    else
    {
        effect->alignment = FC_ALIGN_LEFT;
    }
}

void DrawTextImpl(SDL_Renderer* renderer, FC_Font* font, const char* text, int x, int y, Color color, float scale,
                  TextFlags flags)
{
    FC_Effect effect;

    GetDrawTextProps(color, scale, flags, &effect);

    FC_DrawEffect(font, renderer, static_cast<float>(x), static_cast<float>(y), effect, text);
}

void DrawTextAreaImpl(SDL_Renderer* renderer, FC_Font* font, const char* text, FC_Rect rect, Color color, float scale,
                      TextFlags flags)
{
    FC_Effect effect;

    GetDrawTextProps(color, scale, flags, &effect);

    if (flags & L(TextFlag::Overflow))
    {
        FC_DrawColumnEffect(font, renderer, static_cast<float>(rect.x), static_cast<float>(rect.y),
                            static_cast<Uint16>(rect.w), effect, text);
    }
    else
    {
        FC_DrawBoxEffect(font, renderer, rect, effect, text);
    }
}

DGEX_END

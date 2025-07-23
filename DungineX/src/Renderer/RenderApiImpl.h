/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderApiImpl.h                           *
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

#pragma once

#include "DgeX/Renderer/Color.h"
#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>
#include <SDL_FontCache/SDL_FontCache.h>

DGEX_BEGIN

void SetDrawColorImpl(SDL_Renderer* renderer, const Color& color);

void DrawPointImpl(SDL_Renderer* renderer, int x, int y, Color color);

void DrawLineImpl(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Color color);

void DrawRectImpl(SDL_Renderer* renderer, const Rect& rect, Color color);

void DrawFilledRectImpl(SDL_Renderer* renderer, const Rect& rect, Color color);

void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, const TextureStyle& style, int x, int y);

void DrawTextureImpl(SDL_Renderer* renderer, SDL_Texture* texture, const TextureStyle& style,
                     const TextureAnchor& anchor, int x, int y);

void DrawTextImpl(SDL_Renderer* renderer, FC_Font* font, const char* text, int x, int y, Color color, float scale,
                  TextFlags flags);

void DrawTextAreaImpl(SDL_Renderer* renderer, FC_Font* font, const char* text, FC_Rect rect, Color color, float scale,
                      TextFlags flags);

DGEX_END

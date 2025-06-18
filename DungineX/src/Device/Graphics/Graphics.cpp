/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Graphics.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 3, 2025                              *
 *                                                                            *
 *                    Last Update : June 3, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Graphics device.                                                           *
 ******************************************************************************/

#include "DgeX/Device/Graphics/Graphics.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Device/Graphics/Window.h"
#include "DgeX/Error.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Utils/Log.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

DGEX_BEGIN

dgex_error_t InitGraphics()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        DGEX_CORE_CRITICAL("Failed to initialize SDL: {0}", SDL_GetError());
        return DGEX_ERROR_SDL_INIT;
    }

    if (!TTF_Init())
    {
        DGEX_CORE_CRITICAL("Failed to initialize SDL_ttf: {0}", SDL_GetError());
        return DGEX_ERROR_SDL_TTF_INIT;
    }

    if (dgex_error_t r = InitWindow(); r != DGEX_SUCCESS)
    {
        DGEX_CORE_CRITICAL("Failed to initialize window: {0}", r);
        return r;
    }

    if (dgex_error_t r = InitRenderer(); r != DGEX_SUCCESS)
    {
        DGEX_CORE_CRITICAL("Failed to initialize renderer: {0}", r);
        return r;
    }

    if (dgex_error_t r = InitRenderApi(); r != DGEX_SUCCESS)
    {
        DGEX_CORE_CRITICAL("Failed to initialize render API: {0}", r);
        return r;
    }

    return DGEX_SUCCESS;
}

void DestroyGraphics()
{
    DestroyRenderApi();
    DestroyRenderer();
    DestroyWindow();

    TTF_Quit();
    DGEX_CORE_DEBUG("SDL_ttf destroyed");
    SDL_Quit();
    DGEX_CORE_INFO("SDL destroyed");
}

DGEX_END

/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : MainLoop.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 2, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Main loop of the game.                                                     *
 ******************************************************************************/

#include "Impl/MainLoop.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Utils/Log.h"

#include <SDL3/SDL.h>
#include <SDL_FontCache/SDL_FontCache.h>

DGEX_BEGIN

void MainLoop(OnUpdateCallback onUpdate, OnEventCallback onEvent)
{
    DGEX_CORE_INFO("Main loop started");

    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            onEvent();
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }

        if (onUpdate())
        {
            isRunning = false;
        }
    }

    DGEX_CORE_INFO("Main loop ended");
}

DGEX_END

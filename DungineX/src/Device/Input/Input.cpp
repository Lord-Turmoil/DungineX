/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Input.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : July 31, 2025                             *
 *                                                                            *
 *                    Last Update : July 31, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Direct low-level input handling.                                           *
 ******************************************************************************/

#include "DgeX/Device/Input/Input.h"

#include "DgeX/Device/Graphics/Window.h"

#include <SDL3/SDL.h>

#include <Windows.h>

DGEX_BEGIN

bool IsKeyPressed(KeyCode code)
{
    const bool* states = SDL_GetKeyboardState(nullptr);
    return states[static_cast<int>(code)];
}

bool IsMousePressed(MouseCode code)
{
    SDL_MouseButtonFlags flags = SDL_GetMouseState(nullptr, nullptr);
    return SDL_BUTTON_MASK(code) & flags;
}

// Reference: https://wiki.libsdl.org/SDL3/SDL_GetMouseState
FPoint GetMousePosition()
{
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return FPoint(mouseX, mouseY);
}

DGEX_END

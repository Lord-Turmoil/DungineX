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
 * Direct input handling.                                                     *
 ******************************************************************************/

#include "DgeX/Device/Input/Input.h"

#include "DgeX/Device/Graphics/Window.h"

#include <SDL3/SDL.h>

#include <Windows.h>

DGEX_BEGIN

bool IsKeyPressed(KeyCode key)
{
    return GetAsyncKeyState(static_cast<int>(L(key))) & 0x8000;
}

bool IsMousePressed(MouseCode button)
{
    return GetAsyncKeyState(static_cast<int>(L(button))) & 0x8000;
}

// Reference: https://wiki.libsdl.org/SDL3/SDL_GetMouseState
FPoint GetMousePosition()
{
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return FPoint(mouseX, mouseY);
}

DGEX_END

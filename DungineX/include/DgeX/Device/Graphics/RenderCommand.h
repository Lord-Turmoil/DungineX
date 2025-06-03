/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderCommand.h                           *
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
 * Render command is every single render action.                              *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <SDL3/SDL.h>

DGEX_BEGIN

/**
 * @brief Render command.
 */
class RenderCommand
{
public:
    explicit RenderCommand(int order);
    virtual ~RenderCommand() = default;

    int GetOrder() const;

    virtual void Apply(SDL_Renderer* renderer) = 0;

private:
    int _order;
};

DGEX_END

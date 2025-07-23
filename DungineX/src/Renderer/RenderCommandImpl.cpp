/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderCommandImpl.cpp                     *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 19, 2025                             *
 *                                                                            *
 *                    Last Update : June 19, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Detailed implementation of render command.                                 *
 ******************************************************************************/

#include "Renderer/RenderCommandImpl.h"

#include "DgeX/Renderer/Color.h"

DGEX_BEGIN

NativeRenderCommand::NativeRenderCommand(const std::function<void(SDL_Renderer*)>& action, int order)
    : RenderCommand(order), _action(action)
{
}

void NativeRenderCommand::Apply(SDL_Renderer* renderer)
{
    _action(renderer);
}

Ref<NativeRenderCommand> NativeRenderCommand::Create(const RenderAction& action, int order)
{
    return CreateRef<NativeRenderCommand>(action, order);
}

DGEX_END

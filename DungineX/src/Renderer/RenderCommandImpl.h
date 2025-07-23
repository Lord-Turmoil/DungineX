/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RenderCommandImpl.h                       *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 3, 2025                              *
 *                                                                            *
 *                    Last Update : June 4, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Detailed implementation of render command.                                 *
 ******************************************************************************/

#pragma once

#include "Device/Graphics/RenderCommand.h"

#include "DgeX/Utils/Types.h"

#include <functional>

DGEX_BEGIN

/**
 * @brief Native render command represents a render action.
 *
 * You can have more flexibility to control what you want to render
 * using NativeRenderCommand.
 */
class NativeRenderCommand final : public RenderCommand
{
    using RenderAction = std::function<void(SDL_Renderer*)>;

public:
    NativeRenderCommand(const RenderAction& action, int order);
    NativeRenderCommand(const NativeRenderCommand& other) = delete;
    NativeRenderCommand(NativeRenderCommand&& other) noexcept = delete;
    NativeRenderCommand& operator=(const NativeRenderCommand& other) = delete;
    NativeRenderCommand& operator=(NativeRenderCommand&& other) noexcept = delete;

    ~NativeRenderCommand() override = default;

    void Apply(SDL_Renderer* renderer) override;

    static Ref<NativeRenderCommand> Create(const RenderAction& action, int order = 0);

private:
    RenderAction _action;
};

DGEX_END

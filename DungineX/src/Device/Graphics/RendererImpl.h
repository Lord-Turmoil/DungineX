/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : RendererImpl.h                            *
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
 * Concrete renderer declaration, hidden from the client.                     *
 ******************************************************************************/

#pragma once

#include "DgeX/Device/Graphics/Renderer.h"

#include <vector>

DGEX_BEGIN

/**
 * @brief Execute render commands in the issue order.
 */
class DirectRenderer : public Renderer
{
public:
    DirectRenderer(SDL_Renderer* renderer);
    ~DirectRenderer() override = default;

    void Submit(const Ref<RenderCommand>& command) override;
    void Render() override;
};

/**
 * @brief Execute render commands by their z index.
 */
class OrderedRenderer : public Renderer
{
public:
    OrderedRenderer(SDL_Renderer* renderer);
    ~OrderedRenderer() override = default;

    void Submit(const Ref<RenderCommand>& command) override;
    void Render() override;

private:
    std::vector<Ref<RenderCommand>> _commands;
};

DGEX_END

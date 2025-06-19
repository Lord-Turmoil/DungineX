/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Renderer.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 3, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Detailed renderer implementation.                                          *
 ******************************************************************************/

#pragma once

#include "DgeX/Device/Graphics/Renderer.h"

#include <vector>

DGEX_BEGIN

/**
 * @brief Execute render commands in the issue order.
 */
class DirectRenderer final : public Renderer
{
public:
    DirectRenderer() = default;
    ~DirectRenderer() override = default;

    void Submit(const Ref<RenderCommand>& command) override;

    void Render() override;
};

/**
 * @brief Execute render commands by their z index.
 */
class OrderedRenderer final : public Renderer
{
public:
    OrderedRenderer() = default;
    ~OrderedRenderer() override = default;

    void Submit(const Ref<RenderCommand>& command) override;

    void Render() override;

private:
    std::vector<Ref<RenderCommand>> _commands;
};

DGEX_END

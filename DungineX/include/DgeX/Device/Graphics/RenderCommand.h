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
 * Render command is each single render action.                               *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>

#include <functional>

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
    ~NativeRenderCommand() override = default;

    void Apply(SDL_Renderer* renderer) override;

    static Ref<NativeRenderCommand> Create(const RenderAction& action, int order = 0);

private:
    RenderAction _action;
};

/**
 * @brief Render a prepared texture.
 *
 * If you have a texture ready to go, use TextureRenderCommand to draw
 * it.
 */
class TextureRenderCommand final : public RenderCommand
{
public:
    TextureRenderCommand(SDL_Texture* texture, int x, int y, int z, float scale, float degree, uint8_t alpha);
    ~TextureRenderCommand() override = default;

    void Apply(SDL_Renderer* renderer) override;

    static Ref<TextureRenderCommand> Create(SDL_Texture* texture, int x, int y, int z = 0, float scale = 1.0,
                                            float degree = 0.0, uint8_t alpha = SDL_ALPHA_OPAQUE);

private:
    SDL_Texture* _texture;

    float _x; // x on the screen
    float _y; // y on the screen

    float _scale;   // scale, 1.0 for no scale
    float _degree;  // rotation in degree
    uint8_t _alpha; // alpha value, 0 ~ 255
};

DGEX_END

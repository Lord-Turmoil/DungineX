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

#include "Dgex/Device/Graphics/RenderCommand.h"

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
    // clang-format off
    TextureRenderCommand(
        SDL_Texture* texture,
        float x,
        float y,
        int z,
        float scale, 
        float degree,
        uint8_t alpha,
        bool flipX,
        bool flipY);

    TextureRenderCommand(
        SDL_Texture* texture,
        float x,
        float y,
        int z,
        float anchorX,
        float anchorY,
        float scale, 
        float degree,
        uint8_t alpha,
        bool flipX,
        bool flipY);
    // clang-format on

    ~TextureRenderCommand() override = default;

    void Apply(SDL_Renderer* renderer) override;

private:
    SDL_Texture* _texture;
    SDL_FPoint _anchor;

    float _x; // x on the screen
    float _y; // y on the screen

    float _scale;  // scale, 1.0 for no scale
    float _degree; // rotation in degree

    uint8_t _alpha; // alpha value, 0 ~ 255

    bool _flipX : 1;         // flip horizontally
    bool _flipY : 1;         // flip vertically
    bool _defaultAnchor : 1; // whether to use default anchor or not
};

class TextureRenderCommandBuilder
{
public:
    TextureRenderCommandBuilder(SDL_Texture* texture);

    TextureRenderCommandBuilder& SetAnchor(int x, int y);
    TextureRenderCommandBuilder& SetPosition(int x, int y, int z = 0);
    TextureRenderCommandBuilder& SetScale(float scale);
    TextureRenderCommandBuilder& SetRotation(float degree);
    TextureRenderCommandBuilder& SetAlpha(uint8_t alpha);
    TextureRenderCommandBuilder& FlipX();
    TextureRenderCommandBuilder& FlipY();

    Ref<TextureRenderCommand> Create() const;

private:
    SDL_Texture* _texture;
    SDL_FPoint _anchor;

    float _x; // x on the screen
    float _y; // y on the screen
    int _z;

    float _scale;  // scale, 1.0 for no scale
    float _degree; // rotation in degree

    uint8_t _alpha; // alpha value, 0 ~ 255

    bool _flipX : 1;         // flip horizontally
    bool _flipY : 1;         // flip vertically
    bool _defaultAnchor : 1; // whether to use default anchor
};

DGEX_END

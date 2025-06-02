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

#include "DgeX/Device/Graphics/RenderCommand.h"

DGEX_BEGIN

RenderCommand::RenderCommand(int order) : _order(order)
{
}

int RenderCommand::GetOrder() const
{
    return _order;
}

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

TextureRenderCommand::TextureRenderCommand(SDL_Texture* texture, int x, int y, int z, float scale, float degree,
                                           uint8_t alpha)
    : RenderCommand(z), _texture(texture), _x(static_cast<float>(x)), _y(static_cast<float>(y)), _scale(scale),
      _degree(degree), _alpha(alpha)

{
}

void TextureRenderCommand::Apply(SDL_Renderer* renderer)
{
    SDL_PropertiesID props = SDL_GetTextureProperties(_texture);

    float width = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    float height = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));

    SDL_FRect srcRect{ 0, 0, width, height };

    // Scale the source rectangle around the center of the texture.
    float xOffset = -width * (_scale - 1.0f) * 0.5f;
    float yOffset = -height * (_scale - 1.0f) * 0.5f;
    SDL_FRect dstRect{ _x + xOffset, _y + yOffset, width * _scale + xOffset, height * _scale + yOffset };

    // Set additional alpha.
    SDL_SetTextureAlphaMod(_texture, _alpha);

    // Rotate the texture around the center.
    SDL_RenderTextureRotated(renderer, _texture, &srcRect, &dstRect, _degree, nullptr, SDL_FLIP_NONE);
}

Ref<TextureRenderCommand> TextureRenderCommand::Create(SDL_Texture* texture, int x, int y, int z, float scale,
                                                       float degree, uint8_t alpha)
{
    return CreateRef<TextureRenderCommand>(texture, x, y, z, scale, degree, alpha);
}

DGEX_END

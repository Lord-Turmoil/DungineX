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
#include "Device/Graphics/RenderCommandImpl.h"
#include "DgeX/Renderer/Color.h"

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

TextureRenderCommand::TextureRenderCommand(SDL_Texture* texture, float x, float y, int z, float scale, float degree,
                                           uint8_t alpha, bool flipX, bool flipY)
    : RenderCommand(z), _texture(texture), _anchor(), _x(x), _y(y), _scale(scale), _degree(degree), _alpha(alpha),
      _flipX(flipX), _flipY(flipY), _defaultAnchor(true)
{
}

TextureRenderCommand::TextureRenderCommand(SDL_Texture* texture, float x, float y, int z, float anchorX, float anchorY,
                                           float scale, float degree, uint8_t alpha, bool flipX, bool flipY)
    : RenderCommand(z), _texture(texture), _anchor({ anchorX, anchorY }), _x(x), _y(y), _scale(scale), _degree(degree),
      _alpha(alpha), _flipX(flipX), _flipY(flipY), _defaultAnchor(false)
{
}

void TextureRenderCommand::Apply(SDL_Renderer* renderer)
{
    SDL_PropertiesID props = SDL_GetTextureProperties(_texture);

    float width = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    float height = static_cast<float>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));

    // Scale the source rectangle around the center of the texture.
    float xOffset = -width * (_scale - 1.0f) * 0.5f;
    float yOffset = -height * (_scale - 1.0f) * 0.5f;
    SDL_FRect destRect{ _x + xOffset, _y + yOffset, width * _scale, height * _scale };

    // Set additional alpha.
    SDL_SetTextureAlphaMod(_texture, _alpha);

    // Rotate the texture around the center.
    SDL_FlipMode flip = SDL_FLIP_NONE;
    if (_flipX && _flipY)
    {
        // Flip horizontally and vertically is equivalent to rotate 180deg.
        _degree += 180.0f;
    }
    else if (_flipX)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    else if (_flipY)
    {
        flip = SDL_FLIP_VERTICAL;
    }

    if (_defaultAnchor)
    {
        SDL_FPoint anchor{ destRect.w * 0.5f, destRect.h * 0.5f };
        SDL_RenderTextureRotated(renderer, _texture, nullptr, &destRect, _degree, &anchor, flip);
    }
    else
    {
        _anchor.x *= _scale;
        _anchor.y *= _scale;
        SDL_RenderTextureRotated(renderer, _texture, nullptr, &destRect, _degree, &_anchor, flip);
    }
}

TextureRenderCommandBuilder::TextureRenderCommandBuilder(SDL_Texture* texture)
    : _texture(texture), _anchor(), _x(0), _y(0), _z(0), _scale(1.0f), _degree(0.0f), _alpha(DGEX_COLOR_OPAQUE),
      _flipX(false), _flipY(false), _defaultAnchor(true)
{
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::SetAnchor(int x, int y)
{
    _anchor.x = static_cast<float>(x);
    _anchor.y = static_cast<float>(y);
    _defaultAnchor = false;
    return *this;
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::SetPosition(int x, int y, int z)
{
    _x = static_cast<float>(x);
    _y = static_cast<float>(y);
    _z = z;

    return *this;
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::SetScale(float scale)
{
    _scale = scale;

    return *this;
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::SetRotation(float degree)
{
    _degree = degree;

    return *this;
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::SetAlpha(uint8_t alpha)
{
    _alpha = alpha;

    return *this;
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::FlipX()
{
    _flipX = true;

    return *this;
}

TextureRenderCommandBuilder& TextureRenderCommandBuilder::FlipY()
{
    _flipY = true;

    return *this;
}

Ref<TextureRenderCommand> TextureRenderCommandBuilder::Create() const
{
    if (_defaultAnchor)
    {
        return CreateRef<TextureRenderCommand>(_texture, _x, _y, _z, _scale, _degree, _alpha, _flipX, _flipY);
    }
    return CreateRef<TextureRenderCommand>(_texture, _x, _y, _z, _anchor.x, _anchor.y, _scale, _degree, _alpha, _flipX,
                                           _flipY);
}

DGEX_END

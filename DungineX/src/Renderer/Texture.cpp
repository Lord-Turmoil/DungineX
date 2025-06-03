/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Texture.cpp                               *
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
 * Unified texture interface.                                                 *
 ******************************************************************************/

#include "DgeX/Renderer/Texture.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Utils/Log.h"
#include "SDL3_image/SDL_image.h"

DGEX_BEGIN

Texture::Texture(SDL_Texture* texture) : _texture(texture)
{
}

Texture::Texture(Texture&& other) noexcept
{
    this->_texture = other._texture;
    other._texture = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        _DestroyTexture();
        this->_texture = other._texture;
        other._texture = nullptr;
    }
    return *this;
}

Texture::~Texture()
{
    _DestroyTexture();
}

SDL_Texture* Texture::GetNativeTexture() const
{
    return _texture;
}

int Texture::GetWidth() const
{
    SDL_PropertiesID props = SDL_GetTextureProperties(_texture);
    return static_cast<int>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
}

int Texture::GetHeight() const
{
    SDL_PropertiesID props = SDL_GetTextureProperties(_texture);
    return static_cast<int>(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
}

Ref<Texture> Texture::Create(SDL_Texture* texture)
{
    return CreateRef<Texture>(texture);
}

// Reference: https://wiki.libsdl.org/SDL3/SDL_CreateTexture
Ref<Texture> Texture::Create(int width, int height)
{
    SDL_Texture* texture =
        SDL_CreateTexture(GetNativeRenderer(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height);
    return CreateRef<Texture>(texture);
}

void Texture::_DestroyTexture()
{
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

Ref<Texture> LoadTexture(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        DGEX_CORE_ERROR("Failed to load image: {0}", path);
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(GetNativeRenderer(), surface);
    SDL_DestroySurface(surface);

    return Texture::Create(texture);
}

DGEX_END

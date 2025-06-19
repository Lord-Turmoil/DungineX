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
    _texture = other._texture;
    other._texture = nullptr;
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

SDL_Texture* Texture::GetNativeTexture() const
{
    return _texture;
}

void Texture::Destroy()
{
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
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

Ref<Texture> LoadTexture(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        DGEX_CORE_ERROR("Failed to load texture: {0}, {1}", path, SDL_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(GetNativeRenderer(), surface);
    SDL_DestroySurface(surface);

    DGEX_CORE_INFO("Loaded texture: {0}", path);

    return Texture::Create(texture);
}

Ref<Texture> CreateTexture(int width, int height)
{
    return Texture::Create(width, height);
}

DGEX_END

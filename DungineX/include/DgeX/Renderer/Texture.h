/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Texture.h                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 19, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Unified texture interface.                                                 *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>

#include <string>

DGEX_BEGIN

/**
 * @brief A wrapper for SDL_Texture.
 *
 * We do not allow copying textures, which will lead to uncontrolled
 * texture duplication, making it hard to recycle textures.
 */
class Texture
{
public:
    explicit Texture(SDL_Texture* texture);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = delete;

    ~Texture() = default;

    DGEX_API int GetWidth() const;
    DGEX_API int GetHeight() const;

    SDL_Texture* GetNativeTexture() const;
    void Destroy();

    static Ref<Texture> Create(SDL_Texture* texture);
    static Ref<Texture> Create(int width, int height);

private:
    SDL_Texture* _texture;
};

// ============================================================================
// Utility API
// ----------------------------------------------------------------------------

/**
 * @brief Load texture from file.
 *
 * Currently, support only JPEG, PNG and SVG.
 *
 * @param path Path to the image to load.
 * @return Loaded texture, nullptr on failure.
 */
DGEX_API Ref<Texture> LoadTexture(const std::string& path);

/**
 * @brief Create a plain texture by width and height.
 *
 * Currently, it will create full ARGB texture that can be used as target.
 *
 * @return Texture.
 */
DGEX_API Ref<Texture> CreateTexture(int width, int height);

DGEX_END

/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Font.h                                    *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 8, 2025                              *
 *                                                                            *
 *                    Last Update : June 18, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Font wrapper.                                                              *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Utils/Types.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

DGEX_BEGIN

class Font
{
public:
    explicit Font(TTF_Font* font);
    Font(const Font& other) = delete;
    Font(Font&& other) noexcept = delete;
    Font& operator=(const Font& other) = delete;
    Font& operator=(Font&& other) noexcept = delete;

    ~Font() = default;

    DGEX_API const char* GetName() const;

    TTF_Font* GetNativeFont() const;
    void* GetImpl() const;
    void Destroy();

private:
    TTF_Font* _font;

    // Implementation details hidden.
    void* _impl;
};

/**
 * @brief Load font from file.
 *
 * @param path File path of the font file.
 * @return Loaded font, nullptr on failure.
 */
DGEX_API Ref<Font> LoadFont(const std::string& path);

/**
 * @brief Get required scale for a specific point size.
 *
 * @param pointSize Desired point size.
 * @return The scale value for rendering.
 */
DGEX_API float GetFontScale(float pointSize);

DGEX_END

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
 *                    Last Update : June 8, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Font wrapper.                                                              *
 ******************************************************************************/

#include "DgeX/Renderer/Font.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Utils/Log.h"

#include <SDL_FontCache/SDL_FontCache.h>

#include <filesystem>
#include <unordered_map>

DGEX_BEGIN

static constexpr float DEFAULT_POINT_SIZE = 100.f;
static std::unordered_map<std::string, Ref<Font>> sLoadedFonts;

Font::Font(TTF_Font* font) : _font(font), _impl(FC_CreateFont())
{
    FC_LoadFontFromTTF(static_cast<FC_Font*>(_impl), GetNativeRenderer(), font, FC_MakeColor(0, 0, 0, 255));
    DGEX_CORE_DEBUG("Created font: {0}", GetName());
}

void Font::Destroy()
{
    if (_impl)
    {
        DGEX_CORE_DEBUG("Destroyed font: {0}", GetName());
        FC_FreeFont(static_cast<FC_Font*>(_impl));
        _impl = nullptr;
        _font = nullptr;
    }
}

const char* Font::GetName() const
{
    return TTF_GetFontFamilyName(_font);
}

TTF_Font* Font::GetNativeFont() const
{
    return _font;
}

void* Font::GetImpl() const
{
    return _impl;
}

Ref<Font> Font::Create(TTF_Font* font)
{
    return CreateRef<Font>(font);
}

static Ref<Font> LoadFontImpl(const std::string& path)
{
    std::filesystem::path fontPath = path;

    if (!fontPath.has_extension())
    {
        fontPath += ".ttf";
    }

    TTF_Font* font = TTF_OpenFont(fontPath.string().c_str(), DEFAULT_POINT_SIZE);
    if (font)
    {
        return Font::Create(font);
    }
    if (fontPath.is_absolute())
    {
        DGEX_CORE_WARN("Failed to load font: {0}, {1}", fontPath.string(), SDL_GetError());
        return nullptr;
    }

    // Try relative path in system drive.
    fontPath = "C:/Windows/Fonts" / fontPath;
    font = TTF_OpenFont(fontPath.string().c_str(), DEFAULT_POINT_SIZE);
    if (font)
    {
        return Font::Create(font);
    }
    DGEX_CORE_WARN("Failed to load font: {0}, {1}", fontPath.string(), SDL_GetError());

    return nullptr;
}

Ref<Font> LoadFont(const std::string& path)
{
    Ref<Font> font = LoadFontImpl(path);
    if (!font)
    {
        return font;
    }

    std::string name = font->GetName();
    if (Ref<Font> oldFont = sLoadedFonts[name])
    {
        DGEX_CORE_WARN("Font {0} loaded multiple times, using existing one", name);
        font->Destroy();
        return oldFont;
    }

    sLoadedFonts[name] = font;

    return font;
}

float GetFontScale(float pointSize)
{
    return pointSize / DEFAULT_POINT_SIZE;
}

DGEX_END

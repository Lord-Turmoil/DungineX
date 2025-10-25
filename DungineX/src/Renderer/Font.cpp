/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Font.h                                    *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : June 8, 2025                              *
 *                                                                            *
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Font wrapper.                                                              *
 ******************************************************************************/

#include "DgeX/Renderer/Font.h"

#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Log.h"
#include "DgeX/Utils/Strings.h"

#include <SDL_FontCache/SDL_FontCache.h>
#include <sysfonts/sysfonts.h>

#include <filesystem>
#include <vector>

DGEX_BEGIN

static FontStyles FontStyleFromString(const char* style)
{
    if (Strings::Equals(style, "Bold"))
    {
        return FontStyles::Bold;
    }
    if (Strings::Equals(style, "Italic"))
    {
        return FontStyles::Italic;
    }
    if (Strings::Equals(style, "Bold Italic") || Strings::Equals(style, "BoldItalic"))
    {
        return FontStyles::BoldItalic;
    }
    return FontStyles::Unknown;
}

FontFace::FontFace(TTF_Font* font) : _font(font), _impl(FC_CreateFont())
{
    _name = TTF_GetFontFamilyName(_font);
    _style = FontStyleFromString(TTF_GetFontStyleName(_font));
    FC_LoadFontFromTTF(static_cast<FC_Font*>(_impl), GetNativeRenderer(), font, FC_MakeColor(0, 0, 0, 255));
}

const std::string& FontFace::GetName() const
{
    return _name;
}

FontStyles FontFace::GetStyle() const
{
    return _style;
}

std::string FontFace::GetStyleName() const
{
    return TTF_GetFontStyleName(_font);
}

TTF_Font* FontFace::GetNativeFont() const
{
    return _font;
}

void* FontFace::GetImpl() const
{
    return _impl;
}

void FontFace::Destroy()
{
    if (_impl)
    {
        FC_FreeFont(static_cast<FC_Font*>(_impl));
        _impl = nullptr;
        _font = nullptr;
    }
}

FontFamily::FontFamily(const std::vector<Ref<FontFace>>& fonts)
{
    DGEX_ASSERT(!fonts.empty(), "Font family must have at least one fontface");
    _name = fonts[0]->GetName();
    _fonts = fonts;

    // If there is Regular style, move it to the front.
    for (size_t i = 0; i < _fonts.size(); i++)
    {
        if (_fonts[i]->GetStyle() == FontStyles::Regular)
        {
            if (i != 0)
            {
                std::swap(_fonts[0], _fonts[i]);
            }
            break;
        }
    }
}

const std::string& FontFamily::GetName() const
{
    return _name;
}

Ref<FontFace> FontFamily::GetFont(FontStyles style) const
{
    for (const Ref<FontFace>& font : _fonts)
    {
        if (font->GetStyle() == style)
        {
            return font;
        }
    }
    return _fonts[0];
}

// ============================================================================
// Function implementation.
// ----------------------------------------------------------------------------

static constexpr float DEFAULT_POINT_SIZE = 100.f;

static std::vector<FontFamilyMeta> sFontFamilies;
static Ref<FontFamily> sDefaultFont;

static FontfaceMeta FontInfoToFontfaceMeta(const SF_FontInfo* info);
static void AddFontFromFontfaceMeta(const FontfaceMeta& meta);

static Ref<FontFace> LoadFontfaceFromFile(const std::string& path);
static Ref<FontFace> LoadFontfaceFromMeta(const FontfaceMeta& meta);
static Ref<FontFamily> LoadFontFamilyFromMeta(const FontFamilyMeta& meta);

float GetFontScale(float pointSize)
{
    return pointSize / DEFAULT_POINT_SIZE;
}

static int _SF_Callback(const SF_FontInfo* info, void* context)
{
    DGEX_USED(context);

#ifdef DGEX_PLATFORM_WINDOWS
    // On Windows, loading a font is costly, so we only load selected fonts.
    if (!Strings::StartsWith(info->family, "Arial", "Segoe UI"))
    {
        return SF_CONTINUE;
    }
#endif
    FontfaceMeta meta = FontInfoToFontfaceMeta(info);
    if (meta.Name.empty())
    {
        DGEX_CORE_WARN("Font with empty family name found, ignoring");
        return SF_CONTINUE;
    }
    if (meta.Path.empty())
    {
        DGEX_CORE_WARN("Font {0} with empty path found, ignoring", meta.Name);
        return SF_CONTINUE;
    }

    AddFontFromFontfaceMeta(meta);

    return SF_CONTINUE;
}

dgex_error_t InitFonts()
{
    sFontFamilies.clear();

    if (SF_EnumFonts(_SF_Callback, nullptr) != SF_SUCCESS)
    {
        DGEX_CORE_WARN("Failed to enumerate system fonts: {0}", SF_GetError());
        return DGEX_ERROR_FONT_INIT;
    }

#ifdef DGEX_PLATFORM_WINDOWS
    sDefaultFont = LoadFont("Segoe UI");
#elif defined DGEX_PLATFORM_LINUX
    sDefaultFont = LoadFont("DejaVu Sans");
#elif defined DGEX_PLATFORM_MACOS
    sDefaultFont = LoadFont("Helvetica");
#endif

    if (!sDefaultFont)
    {
        DGEX_CORE_WARN("Default font not found, using the first available font");
        const std::vector<FontFamilyMeta>& families = GetAvailableFontFamilies();
        if (families.empty())
        {
            DGEX_CORE_ERROR("No available font found");
            return DGEX_ERROR_FONT_INIT;
        }
        sDefaultFont = LoadFont(families.front().Name);
    }
    if (!sDefaultFont)
    {
        DGEX_CORE_ERROR("Failed to load default font");
        return DGEX_ERROR_FONT_INIT;
    }

    return DGEX_SUCCESS;
}

dgex_error_t AddFont(const std::string& path, const std::string& name)
{
    Ref<FontFace> font = LoadFontfaceFromFile(path);
    if (!font)
    {
        return DGEX_ERROR;
    }

    FontfaceMeta meta = { name.empty() ? font->GetName() : name, std::filesystem::path(path), font->GetStyle() };
    AddFontFromFontfaceMeta(meta);

    font->Destroy();

    return DGEX_SUCCESS;
}

Ref<FontFamily> LoadFont(const std::string& name)
{
    for (const FontFamilyMeta& meta : sFontFamilies)
    {
        if (meta.Name == name)
        {
            return LoadFontFamilyFromMeta(meta);
        }
    }
    DGEX_CORE_WARN("Font family {0} not found", name);

    return nullptr;
}

const std::vector<FontFamilyMeta>& GetAvailableFontFamilies()
{
    return sFontFamilies;
}

const Ref<FontFamily>& GetDefaultFont()
{
    return sDefaultFont;
}

// ============================================================================
// Internal function implementation.
// ----------------------------------------------------------------------------

FontfaceMeta FontInfoToFontfaceMeta(const SF_FontInfo* info)
{
#ifdef DGEX_PLATFORM_WINDOWS
    Ref<FontFace> font = LoadFontfaceFromFile(info->path);
    if (!font)
    {
        return { "", std::filesystem::path(), FontStyles::Regular };
    }
    FontfaceMeta meta = { font->GetName(), std::filesystem::path(info->path), font->GetStyle() };
    font->Destroy();
    return meta;
#else
    return {
        info->family ? info->family : "",
        info->path ? std::filesystem::path(info->path) : std::filesystem::path(),
        info->style ? FontStyleFromString(info->style) : FontStyles::Regular,
    };
#endif
}

void AddFontFromFontfaceMeta(const FontfaceMeta& meta)
{
    for (auto& [Name, Fonts] : sFontFamilies)
    {
        if (Name == meta.Name)
        {
            for (const FontfaceMeta& fontface : Fonts)
            {
                if (fontface.Style == meta.Style)
                {
                    DGEX_CORE_WARN("Duplicated style {0} in font {1}, ignoring", ToString(meta.Style), meta.Name);
                    return;
                }
            }
            Fonts.push_back(meta);
            return;
        }
    }

    sFontFamilies.emplace_back(FontFamilyMeta{ meta.Name, { meta } });
}

Ref<FontFace> LoadFontfaceFromFile(const std::string& path)
{
    std::filesystem::path fontPath = path;

    if (!fontPath.has_extension())
    {
        fontPath += ".ttf";
    }

    if (TTF_Font* font = TTF_OpenFont(fontPath.string().c_str(), DEFAULT_POINT_SIZE))
    {
        return CreateRef<FontFace>(font);
    }

    DGEX_CORE_ERROR("Failed to load font: {0}, {1}", fontPath.string(), SDL_GetError());

    return nullptr;
}

Ref<FontFace> LoadFontfaceFromMeta(const FontfaceMeta& meta)
{
    return LoadFontfaceFromFile(meta.Path.string());
}

Ref<FontFamily> LoadFontFamilyFromMeta(const FontFamilyMeta& meta)
{
    std::vector<Ref<FontFace>> fonts;

    for (const FontfaceMeta& fontfaceMeta : meta.Fonts)
    {
        if (Ref<FontFace> fontface = LoadFontfaceFromMeta(fontfaceMeta))
        {
            fonts.push_back(fontface);
        }
    }

    if (fonts.empty())
    {
        DGEX_CORE_WARN("No valid fontface found in font family {0}", meta.Name);
    }

    return CreateRef<FontFamily>(fonts);
}

DGEX_END
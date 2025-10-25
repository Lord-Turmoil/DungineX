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

#pragma once

#include "DgeX/Error.h"
#include "DgeX/Utils/Types.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <filesystem>
#include <string>
#include <vector>

DGEX_BEGIN

enum class FontStyles : uint8_t
{
    Regular = 0,
    Bold,
    Italic,
    BoldItalic,
    Unknown
};

inline std::string ToString(FontStyles style)
{
    switch (style)
    {
    case FontStyles::Regular:
        return "Regular";
    case FontStyles::Bold:
        return "Bold";
    case FontStyles::Italic:
        return "Italic";
    case FontStyles::BoldItalic:
        return "BoldItalic";
    case FontStyles::Unknown:
        return "Unknown";
    }
    return "Unknown";
}

struct FontfaceMeta
{
    std::string Name;
    std::filesystem::path Path;
    FontStyles Style;
};

struct FontFamilyMeta
{
    std::string Name;
    std::vector<FontfaceMeta> Fonts;
};

/**
 * @brief A simple font wrapper.
 */
class FontFace
{
public:
    explicit FontFace(TTF_Font* font);

    FontFace(const FontFace& other) = delete;
    FontFace(FontFace&& other) noexcept = delete;
    FontFace& operator=(const FontFace& other) = delete;
    FontFace& operator=(FontFace&& other) noexcept = delete;

    ~FontFace() = default;

    /**
     * @brief Get the name of the font face.
     *
     * @return The name of the font face.
     */
    DGEX_API const std::string& GetName() const;

    /**
     * @brief Get the style of the font face.
     *
     * @return The style of the font face.
     */
    DGEX_API FontStyles GetStyle() const;

    /**
     * @brief Get the style name of the font face.
     *
     * This may differ from the style enum as the style name may be more descriptive.
     *
     * @return The actual style name of the font face.
     */
    DGEX_API std::string GetStyleName() const;

    TTF_Font* GetNativeFont() const;

    void* GetImpl() const;

    void Destroy();

private:
    std::string _name;
    FontStyles _style;

    TTF_Font* _font;

    // Implementation details hidden.
    void* _impl;
};

/**
 * @brief A collection of fonts.
 *
 * A font family at least has one font.
 */
class FontFamily
{
public:
    explicit FontFamily(const std::vector<Ref<FontFace>>& fonts);
    FontFamily(const FontFamily&) = delete;
    FontFamily(FontFamily&&) noexcept = delete;
    FontFamily& operator=(const FontFamily&) = delete;
    FontFamily& operator=(FontFamily&&) noexcept = delete;

    /**
     * @brief Get the name of the font family.
     *
     * @return The name of the font family.
     */
    const std::string& GetName() const;

    /**
     * @brief Get the font with the given style.
     *
     * @param style Font with the given style.
     * @return The font, or default style if the style not found.
     */
    Ref<FontFace> GetFont(FontStyles style) const;

private:
    std::string _name;
    std::vector<Ref<FontFace>> _fonts;
};

/**
 * @brief Get required scale for a specific point size.
 *
 * @param pointSize Desired point size.
 * @return The scale value for rendering.
 */
DGEX_API float GetFontScale(float pointSize);

/**
 * @brief Initialize system font cache.
 *
 * This will scan all installed system fonts for their metadata.
 * It is required to load fonts.
 *
 * @todo Maybe we can move this time-consuming operation to a separate thread.
 *
 * @return 0 on success, otherwise failure.
 */
DGEX_API dgex_error_t InitFonts();

/**
 * @brief Add custom font from file into font cache.
 *
 * One can only load a font that is added to the font cache.
 *
 * @param path Path to the font file.
 * @param name Override the name of the font family, leave empty to use the default name.
 * @return 0 on success, otherwise failure.
 */
DGEX_API dgex_error_t AddFont(const std::string& path, const std::string& name = "");

/**
 * @brief Load a font family by name.
 *
 * @param name Name of the font family.
 * @return Loaded font family, nullptr if failed.
 */
DGEX_API Ref<FontFamily> LoadFont(const std::string& name);

/**
 * @brief Get all available font families.
 *
 * @return All available fonts.
 */
DGEX_API const std::vector<FontFamilyMeta>& GetAvailableFontFamilies();

/**
 * @brief Get the default font on the system.
 *
 * At least there should be one font available.
 *
 * @return The default font on the system.
 */
DGEX_API const Ref<FontFamily>& GetDefaultFont();

DGEX_END
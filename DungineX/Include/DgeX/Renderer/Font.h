#pragma once

#include "DgeX/Common/Common.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>

DGEX_BEGIN

class Texture;

struct MsdfData
{
    std::vector<msdf_atlas::GlyphGeometry> Glyphs;
    msdf_atlas::FontGeometry FontGeometry;
};

class Font
{
public:
    Font(const std::filesystem::path& path);
    ~Font();

    const MsdfData* GetMsdfData() const
    {
        return _data;
    }

    Ref<Texture> GetAtlasTexture() const
    {
        return _atlasTexture;
    }

    static Ref<Font> Load(const std::filesystem::path& path);

private:
    MsdfData* _data;
    Ref<Texture> _atlasTexture;
};

/**
 * @brief
 * A class to hold all loaded fonts.
 */
class FontRegistry
{
public:
    static void Init();
    static void Shutdown();

    static void Add(const std::string& fontName, const Ref<Font>& font);
    static Ref<Font> Load(const std::string& fontName, const std::filesystem::path& path);
    static void UnLoad(const std::string& fontName);
    static Ref<Font> Get(const std::string& fontName);
    static Ref<Font> GetDefault();

private:
    static std::unordered_map<std::string, Ref<Font>> _sFonts;
};

DGEX_END

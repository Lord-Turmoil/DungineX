#pragma once

#include "DgeX/dgexpch.h"

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
    Font(std::string name, const std::filesystem::path& path);
    ~Font();

    const std::string& GetName() const
    {
        return _name;
    }

    const MsdfData* GetMsdfData() const
    {
        return _data;
    }

    Ref<Texture> GetAtlasTexture() const
    {
        return _atlasTexture;
    }

    static Ref<Font> Load(const std::string& name, const std::filesystem::path& path);

private:
    std::string _name;
    Ref<Texture> _atlasTexture;
    MsdfData* _data;
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

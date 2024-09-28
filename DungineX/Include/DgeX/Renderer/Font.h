#pragma once

#include <DgeX/Renderer/Texture.h>

#undef INFINITE
#include <msdf-atlas-gen/msdf-atlas-gen.h>

DGEX_BEGIN

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

    static Ref<Font> GetDefault();
    static Ref<Font> Load(const std::filesystem::path& path);

private:
    MsdfData* _data;
    Ref<Texture> _atlasTexture;
};

DGEX_END

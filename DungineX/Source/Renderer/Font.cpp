/**
 * Original code from Hazel Engine.
 */

#include "DgeX/Renderer/Font.h"

DGEX_BEGIN

template <typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
static Ref<Texture> CreateAndCacheAtlas(const std::string& fontName, float fontSize,
                                        const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
                                        const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
{
    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = true;
    attributes.scanlinePass = true;

    msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
    generator.setAttributes(attributes);
    generator.setThreadCount(8);
    generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

    msdfgen::BitmapConstRef<T, N> bitmap = static_cast<msdfgen::BitmapConstRef<T, N>>(generator.atlasStorage());

    TextureSpecification spec;
    spec.Width = bitmap.width;
    spec.Height = bitmap.height;
    spec.Format = ImageFormat::RGB8;
    spec.GenerateMips = false;

    Ref<Texture> texture = Texture::Create(spec);
    texture->SetData(static_cast<const void*>(bitmap.pixels), bitmap.width * bitmap.height * 3);

    return texture;
}

Font::Font(const std::filesystem::path& filepath) : _data(new MsdfData())
{
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
    DGEX_ASSERT(ft);

    std::string fileString = filepath.string();
    msdfgen::FontHandle* font = loadFont(ft, fileString.c_str());
    if (!font)
    {
        DGEX_CORE_ERROR("Failed to load font: {}", fileString);
        return;
    }

    struct CharsetRange
    {
        uint32_t Begin, End;
    };

    // Currently only support ASCII characters.
    static constexpr CharsetRange sCharsetRanges[] = { { 0x0020, 0x00FF } };

    msdf_atlas::Charset charset;
    for (auto [Begin, End] : sCharsetRanges)
    {
        for (uint32_t c = Begin; c <= End; c++)
        {
            charset.add(c);
        }
    }

    double fontScale = 1.0;
    _data->FontGeometry = msdf_atlas::FontGeometry(&_data->Glyphs);
    int glyphsLoaded = _data->FontGeometry.loadCharset(font, fontScale, charset);
    DGEX_CORE_INFO("Loaded {}/{} glyphs from font {}", glyphsLoaded, charset.size(), fileString);

    double emSize = 48.0f;
    msdf_atlas::TightAtlasPacker atlasPacker;
    atlasPacker.setPixelRange(2.0);
    atlasPacker.setMiterLimit(1.0);
    atlasPacker.setPadding(0);
    atlasPacker.setScale(emSize);
    int remaining = atlasPacker.pack(_data->Glyphs.data(), static_cast<int>(_data->Glyphs.size()));
    DGEX_ASSERT(remaining == 0);

    int width, height;
    atlasPacker.getDimensions(width, height);
    emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER          6364136223846793005ull
#define LCG_INCREMENT           1442695040888963407ull
#define THREAD_COUNT            8

    unsigned long long glyphSeed = 0;
    for (msdf_atlas::GlyphGeometry& glyph : _data->Glyphs)
    {
        glyphSeed *= LCG_MULTIPLIER;
        glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
    }
    _atlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>(
        "", static_cast<float>(emSize), _data->Glyphs, _data->FontGeometry, width, height);

    destroyFont(font);
    deinitializeFreetype(ft);

    DGEX_CORE_INFO("Successfully loaded font: {}", fileString);
}

Font::~Font()
{
    delete _data;
}

Ref<Font> Font::GetDefault()
{
    static Ref<Font> sDefaultFont;
    if (!sDefaultFont)
    {
        sDefaultFont = Load("C:/Windows/Fonts/segoeui.ttf");
    }

    DGEX_ASSERT(sDefaultFont, "Default font not available!");

    return sDefaultFont;
}

Ref<Font> Font::Load(const std::filesystem::path& path)
{
    return CreateRef<Font>(path);
}

DGEX_END

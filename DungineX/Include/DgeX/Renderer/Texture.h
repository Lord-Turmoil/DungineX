#pragma once

#include "DgeX/dgexpch.h"

DGEX_BEGIN

enum class ImageFormat : uint8_t
{
    None = 0,
    R8,
    RGB8,
    RGBA8,
    RGBA32F
};

inline const char* ImageFormatToString(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::None:
        return "None";
    case ImageFormat::R8:
        return "R8";
    case ImageFormat::RGB8:
        return "RGB8";
    case ImageFormat::RGBA8:
        return "RGBA8";
    case ImageFormat::RGBA32F:
        return "RGBA32F";
    default:
        return "unknown";
    }
}

struct TextureSpecification
{
    uint32_t Width = 1;
    uint32_t Height = 1;
    ImageFormat Format = ImageFormat::RGBA8;
    bool GenerateMips = true;
};

/**
 * @brief Represent a texture object.
 * @note Currently only support 2D texture.
 */
class Texture
{
public:
    virtual ~Texture() = default;

    virtual const TextureSpecification& GetSpecification() const = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    virtual const std::string& GetPath() const = 0;

    virtual void SetData(void* data, uint32_t size) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual bool IsLoaded() const = 0;

    virtual bool operator==(const Texture& other) const = 0;

    static Ref<Texture> Create(const TextureSpecification& specification);
    static Ref<Texture> Create(const std::string& path);
};

DGEX_END

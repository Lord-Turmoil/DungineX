#pragma once

#include "DgeX/dgexpch.h"

DGEX_BEGIN

enum class ImageFormat
{
    None = 0,
    R8,
    RGB8,
    RGBA8,
    RGBA32F
};

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
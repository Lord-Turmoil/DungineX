#pragma once

#include "DgeX/Renderer/Texture.h"

#ifdef DGEX_OPENGL

#include <glad/glad.h>

DGEX_BEGIN

class OpenGLTexture : public Texture
{
public:
    OpenGLTexture(const TextureSpecification& specification);
    OpenGLTexture(const std::string& path);
    ~OpenGLTexture() override;

    const TextureSpecification& GetSpecification() const override
    {
        return _specification;
    }

    uint32_t GetWidth() const override
    {
        return _width;
    }

    uint32_t GetHeight() const override
    {
        return _height;
    }

    uint32_t GetRendererID() const override
    {
        return _rendererId;
    }

    const std::string& GetPath() const override
    {
        return _path;
    }

    void SetData(const void* data, uint32_t size) override;

    void Bind(uint32_t slot = 0) const override;

    bool IsLoaded() const override
    {
        return _isLoaded;
    }

    bool operator==(const Texture& other) const override
    {
        return _rendererId == other.GetRendererID();
    }

private:
    TextureSpecification _specification;
    uint32_t _rendererId;
    GLenum _internalFormat;
    GLenum _dataFormat;

    std::string _path;
    uint32_t _width;
    uint32_t _height;

    bool _isLoaded = false;
};

DGEX_END

#endif

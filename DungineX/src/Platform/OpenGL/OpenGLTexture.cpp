#include "DgeX/Platform/OpenGL/OpenGLTexture.h"

#include "DgeX/Common/Assert.h"

#include <stb/stb_image.h>

DGEX_BEGIN

static GLenum ImageFormatToGLDataFormat(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::RGB8:
        return GL_RGB;
    case ImageFormat::RGBA8:
        return GL_RGBA;
    default:
        DGEX_ASSERT(false, DGEX_MSG_IMAGE_INVALID_FORMAT);
    }
    return 0;
}

static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::RGB8:
        return GL_RGB8;
    case ImageFormat::RGBA8:
        return GL_RGBA8;
    default:
        DGEX_ASSERT(false, DGEX_MSG_IMAGE_INVALID_FORMAT);
    }
    return 0;
}

OpenGLTexture::OpenGLTexture(const TextureSpecification& specification)
    : _specification(specification), _width(_specification.Width), _height(_specification.Height)
{
    _internalFormat = ImageFormatToGLInternalFormat(_specification.Format);
    _dataFormat = ImageFormatToGLDataFormat(_specification.Format);

    glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
    glTextureStorage2D(_rendererId, 1, _internalFormat, _width, _height);

    glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

    DGEX_CORE_INFO("Texture created: {0}x{1}, format: {2}", _width, _height,
                   ImageFormatToString(_specification.Format));
}

OpenGLTexture::OpenGLTexture(const std::string& path)
    : _rendererId(0), _internalFormat(0), _dataFormat(0), _path(path), _width(0), _height(0)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data)
    {
        _isLoaded = true;

        _width = width;
        _height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        _internalFormat = internalFormat;
        _dataFormat = dataFormat;

        DGEX_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
        glTextureStorage2D(_rendererId, 1, internalFormat, _width, _height);

        glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    DGEX_CORE_INFO("Texture loaded: {0}", path);
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &_rendererId);
    DGEX_CORE_INFO("Texture deleted: {0}", _path);
}

void OpenGLTexture::SetData(const void* data, uint32_t size)
{
    uint32_t bpp = _dataFormat == GL_RGBA ? 4 : 3;
    DGEX_ASSERT(size == _width * _height * bpp, DGEX_MSG_TEXTURE_DATA_SIZE);
    glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, _rendererId);
}

DGEX_END

#include "DgeX/Renderer/Texture.h"
#include "DgeX/Platform/OpenGL/OpenGLTexture.h"

DGEX_BEGIN

Ref<Texture> Texture::Create(const TextureSpecification& specification)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLTexture>(specification);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

Ref<Texture> Texture::Create(const std::string& path)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLTexture>(path);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END

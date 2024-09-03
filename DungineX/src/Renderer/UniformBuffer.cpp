#include "DgeX/Renderer/UniformBuffer.h"

#include "DgeX/Platform/OpenGL/OpenGLUniformBuffer.h"

DGEX_BEGIN

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLUniformBuffer>(size, binding);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END

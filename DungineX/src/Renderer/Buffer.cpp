#include "DgeX/Renderer/Buffer.h"

#include "DgeX/Renderer/OpenGL/OpenGLBuffer.h"

DGEX_BEGIN

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLVertexBuffer>(size);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLVertexBuffer>(vertices, size);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLIndexBuffer>(indices, count);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END

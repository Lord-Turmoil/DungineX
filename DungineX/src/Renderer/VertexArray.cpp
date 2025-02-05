#include "DgeX/Renderer/VertexArray.h"

#include "DgeX/Renderer/OpenGL/OpenGLVertexArray.h"

DGEX_BEGIN

Ref<VertexArray> VertexArray::Create()
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLVertexArray>();
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END

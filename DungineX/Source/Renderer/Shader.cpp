#include "DgeX/Renderer/Shader.h"
#include "DgeX/Platform/OpenGL/OpenGLShader.h"

DGEX_BEGIN

Ref<Shader> Shader::Create(const std::string& filepath)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLShader>(filepath);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

Ref<Shader> Shader::Create(const std::string& name, const char* vertexShaderSource, const char* fragmentShaderSource)
{
#ifdef DGEX_OPENGL
    return CreateRef<OpenGLShader>(name, vertexShaderSource, fragmentShaderSource);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END

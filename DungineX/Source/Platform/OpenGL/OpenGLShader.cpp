// Reference:
//   - https://learnopengl.com/Getting-started/Shaders

#include "DgeX/Platform/OpenGL/OpenGLShader.h"
#include "DgeX/Utils/File.h"

#include <fstream>
#include <glad/glad.h>

#ifdef DGEX_OPENGL

DGEX_BEGIN

static GLenum ParseShaderTypeFromString(const std::string& type)
{
    if (type.find("vertex") != std::string::npos)
    {
        return GL_VERTEX_SHADER;
    }
    if (type.find("fragment") != std::string::npos)
    {
        return GL_FRAGMENT_SHADER;
    }
    DGEX_ASSERT(false, "Unknown shader type!");
    return 0;
}

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
    {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment")
    {
        return GL_FRAGMENT_SHADER;
    }
    DGEX_ASSERT(false, "Unknown shader type!");
    return 0;
}

static const char* GLShaderStageToString(GLenum stage)
{
    switch (stage)
    {
    case GL_VERTEX_SHADER:
        return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
        return "GL_FRAGMENT_SHADER";
    default:
        DGEX_ASSERT(false, "Invalid shader stage");
    }
    return nullptr;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
    std::string vertexSource;
    std::string fragmentSource;
    std::string name = _ParseShaderFile(filepath, vertexSource, fragmentSource);
    if (name.empty())
    {
        DGEX_CORE_ERROR(DGEX_MSG_SHADER_LOAD_ERROR, filepath);
        return;
    }
    _Init(name, vertexSource.c_str(), fragmentSource.c_str());
}

OpenGLShader::OpenGLShader(const std::string& name, const char* vertexShaderSource, const char* fragmentShaderSource)
{
    _Init(name, vertexShaderSource, fragmentShaderSource);
}

OpenGLShader::~OpenGLShader()
{
    if (_programId != 0)
    {
        glDeleteShader(_programId);
    }
    DGEX_LOG_INFO("Shader {0} unloaded", _name);
}

void OpenGLShader::Bind() const
{
    if (_programId != 0)
    {
        glUseProgram(_programId);
    }
}

void OpenGLShader::Unbind() const
{
    if (_programId != 0)
    {
        glUseProgram(0);
    }
}

void OpenGLShader::SetInt(const std::string& name, int value)
{
    if (_programId != 0)
    {
        glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
    }
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, int count)
{
    if (_programId != 0)
    {
        glUniform1iv(glGetUniformLocation(_programId, name.c_str()), count, values);
    }
}

void OpenGLShader::SetFloat(const std::string& name, float value)
{
    if (_programId != 0)
    {
        glUniform1f(glGetUniformLocation(_programId, name.c_str()), value);
    }
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
{
    if (_programId != 0)
    {
        glUniform2f(glGetUniformLocation(_programId, name.c_str()), value.x, value.y);
    }
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
    if (_programId != 0)
    {
        glUniform3f(glGetUniformLocation(_programId, name.c_str()), value.x, value.y, value.z);
    }
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
    if (_programId != 0)
    {
        glUniform4f(glGetUniformLocation(_programId, name.c_str()), value.x, value.y, value.z, value.w);
    }
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
    if (_programId != 0)
    {
        glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
}

void OpenGLShader::_Init(const std::string& name, const char* vertexShaderSource, const char* fragmentShaderSource)
{
    _name = name;

    if (!vertexShaderSource)
    {
        DGEX_CORE_WARN("No vertex shader source provided, using default vertex shader");
        vertexShaderSource = ShaderTemplate::DEFAULT_VERTEX_SHADER;
    }

    if (!fragmentShaderSource)
    {
        DGEX_CORE_WARN("No fragment shader source provided, using default fragment shader");
        fragmentShaderSource = ShaderTemplate::DEFAULT_FRAGMENT_SHADER;
    }

    // vertex shader
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex);
    if (!_CheckCompileErrors(vertex, "VERTEX"))
    {
        DGEX_LOG_ERROR(DGEX_MSG_SHADER_COMPILE_ERROR, _name);
        glDeleteShader(vertex);
        return;
    }

    // fragment Shader
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment);
    if (!_CheckCompileErrors(fragment, "FRAGMENT"))
    {
        DGEX_LOG_ERROR(DGEX_MSG_SHADER_COMPILE_ERROR, _name);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    // shader Program
    _programId = glCreateProgram();
    glAttachShader(_programId, vertex);
    glAttachShader(_programId, fragment);
    glLinkProgram(_programId);
    if (!_CheckCompileErrors(_programId, "PROGRAM"))
    {
        DGEX_LOG_ERROR(DGEX_MSG_SHADER_LINK_ERROR, _name);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(_programId);
        _programId = 0;
        return;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    DGEX_LOG_INFO("Shader {0} loaded", _name);
}

/**
 * @brief Parse shader from a joint declaration.
 * @param filepath path of the shader file
 * @param vertexSource return the vertex shader source
 * @param fragmentSource return the fragment shader source
 * @return name of the shader, extracted from filepath. "" if failed to open file.
 * @note The shader file should be in the format:
 *          #shader vertex
 *              <vertex shader source>
 *          #shader fragment
 *              <fragment shader source>
 * @note Vertex shader and fragment shader can be in any order.
 * @note Empty lines will be omitted.
 */
std::string OpenGLShader::_ParseShaderFile(const std::string& filepath, std::string& vertexSource,
                                           std::string& fragmentSource)
{
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (!in)
    {
        DGEX_CORE_ERROR(DGEX_MSG_CANNOT_OPEN_FILE, filepath);
        return "";
    }

    std::string line;
    GLenum type = 0;
    std::stringstream source;
    while (std::getline(in, line))
    {
        if (line.empty() || (line.back() == '\r')) // Skip empty lines
        {
            continue;
        }

        if (line.rfind("#shader", 0) == 0)
        {
            if (type == GL_VERTEX_SHADER)
            {
                vertexSource = source.str();
            }
            else if (type == GL_FRAGMENT_SHADER)
            {
                fragmentSource = source.str();
            }
            type = ParseShaderTypeFromString(line);
            if (type == 0)
            {
                DGEX_CORE_ERROR(DGEX_MSG_SHADER_SYNTAX_ERROR, line);
                return "";
            }
            source.clear();
            continue;
        }

        source << line << '\n';
    }

    if (type == GL_VERTEX_SHADER)
    {
        vertexSource = source.str();
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        fragmentSource = source.str();
    }

    return Utils::File::GetFileName(filepath);
}

bool OpenGLShader::_CheckCompileErrors(uint32_t shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            DGEX_CORE_ERROR("Shader compilation error of type: {0}\n{1}", type, infoLog);
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            DGEX_CORE_ERROR("Shader linking error of type: {0}\n{1}", type, infoLog);
            return false;
        }
    }

    return true;
}

namespace ShaderTemplate
{

const char* DEFAULT_VERTEX_SHADER = R"(
#version 330 core
layout(location = 0) in vec2 _pos;
int main()
{
    gl_Position = vec4(_pos, 0.0, 1.0);
}
)";

const char* DEFAULT_FRAGMENT_SHADER = R"(
#version 330 core
out vec4 __frag_color;
void main()
{
    __frag_color = vec4(1.0, 0.0, 1.0, 1.0);
}
)";

const char* STANDARD_FRAGMENT_SHADER = R"(
#version 330 core
out vec4 __frag_color;
uniform vec4  _color;
void main()
{
    __frag_color = _color;
}
)";

} // namespace ShaderTemplate

DGEX_END

#endif

#include "DgeX/Platform/OpenGL/OpenGLVertexArray.h"

#ifdef DGEX_OPENGL

#include <glad/glad.h>

DGEX_BEGIN

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    default:
        DGEX_ASSERT(false, "Unknown ShaderDataType!");
    }

    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &_rendererId);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &_rendererId);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(_rendererId);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
    DGEX_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(_rendererId);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
        switch (element.Type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4: {
            glEnableVertexAttribArray(_vertexBufferIndex);
            glVertexAttribPointer(_vertexBufferIndex, element.GetComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(), (const void*)element.Offset);
            _vertexBufferIndex++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool: {
            glEnableVertexAttribArray(_vertexBufferIndex);
            glVertexAttribIPointer(_vertexBufferIndex, element.GetComponentCount(),
                                   ShaderDataTypeToOpenGLBaseType(element.Type), layout.GetStride(),
                                   (const void*)element.Offset);
            _vertexBufferIndex++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
            uint8_t count = element.GetComponentCount();
            for (uint8_t i = 0; i < count; i++)
            {
                glEnableVertexAttribArray(_vertexBufferIndex);
                glVertexAttribPointer(_vertexBufferIndex, count, ShaderDataTypeToOpenGLBaseType(element.Type),
                                      element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                                      (const void*)(element.Offset + sizeof(float) * count * i));
                glVertexAttribDivisor(_vertexBufferIndex, 1);
                _vertexBufferIndex++;
            }
            break;
        }
        default:
            DGEX_ASSERT(false, "Unknown ShaderDataType!");
        }
    }

    _vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
    glBindVertexArray(_rendererId);
    indexBuffer->Bind();
    _indexBuffer = indexBuffer;
}

DGEX_END

#endif

#pragma once

#include "DgeX/Common/Common.h"

DGEX_BEGIN

// clang-format off
enum class ShaderDataType
{
    None = 0,
    Float, Float2, Float3, Float4,
    Mat3,  Mat4,
    Int,   Int2,   Int3,   Int4,
    Bool
};

// clang-format on

static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Float2:
        return 4 * 2;
    case ShaderDataType::Float3:
        return 4 * 3;
    case ShaderDataType::Float4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Bool:
        return 1;
    default:
        DGEX_ASSERT(false, "Unknown ShaderDataType!");
    }

    return 0;
}

struct BufferElement
{
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    size_t Offset;
    bool Normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType type, std::string name, bool normalized = false)
        : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
    {
    }

    uint8_t GetComponentCount() const
    {
        switch (Type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3; // 3 * float3
        case ShaderDataType::Mat4:
            return 4; // 4 * float4
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            DGEX_ASSERT(false, "Unknown ShaderDataType!");
        }

        return 0;
    }
};

// clang-format off
class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(std::initializer_list<BufferElement> elements) : _elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    uint32_t GetStride() const { return _stride; }

    const std::vector<BufferElement>& GetElements() const { return _elements; }
    std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
    std::vector<BufferElement>::iterator end() { return _elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

private:
    void CalculateOffsetsAndStride()
    {
        int offset = 0;
        _stride = 0;
        for (auto& element : _elements)
        {
            element.Offset = offset;
            offset += element.Size;
            _stride += element.Size;
        }
    }

private:
    std::vector<BufferElement> _elements;
    uint32_t _stride = 0;
};

// clang-format on

class VertexBuffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept = delete;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept = delete;
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static Ref<VertexBuffer> Create(uint32_t size);
    static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
    IndexBuffer() = default;
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) noexcept = delete;
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept = delete;
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
};

DGEX_END

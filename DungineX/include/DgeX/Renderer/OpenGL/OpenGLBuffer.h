#pragma once

#include "DgeX/Renderer/Buffer.h"

DGEX_BEGIN

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    void SetData(const void* data, uint32_t size) override;

    const BufferLayout& GetLayout() const override
    {
        return _layout;
    }

    void SetLayout(const BufferLayout& layout) override
    {
        _layout = layout;
    }

private:
    uint32_t _rendererId;
    BufferLayout _layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t GetCount() const override
    {
        return _count;
    }

private:
    uint32_t _rendererId;
    uint32_t _count;
};

DGEX_END

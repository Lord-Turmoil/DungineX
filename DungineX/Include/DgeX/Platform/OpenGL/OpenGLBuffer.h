#pragma once

#include "DgeX/Renderer/Buffer.h"

DGEX_BEGIN

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(int size);
    OpenGLVertexBuffer(float* vertices, int size);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    void SetData(const void* data, int size) override;

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
    OpenGLIndexBuffer(int* indices, int count);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    int GetCount() const override
    {
        return _count;
    }

private:
    uint32_t _rendererId;
    int _count;
};

DGEX_END

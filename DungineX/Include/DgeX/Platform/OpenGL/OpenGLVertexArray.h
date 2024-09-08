#pragma once

#include "DgeX/Renderer/VertexArray.h"

DGEX_BEGIN

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

private:
    uint32_t _rendererId;
    uint32_t _vertexBufferIndex = 0;
    std::vector<Ref<VertexBuffer>> _vertexBuffers;
    Ref<IndexBuffer> _indexBuffer;
};

DGEX_END

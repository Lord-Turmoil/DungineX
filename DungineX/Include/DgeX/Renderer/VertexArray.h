#pragma once

#include "DgeX/Renderer/Buffer.h"

DGEX_BEGIN

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

    uint32_t GetVertexCount() const
    {
        return GetIndexBuffer()->GetCount();
    }

    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

    uint32_t GetIndexCount() const
    {
        return GetIndexBuffer()->GetCount();
    }

    static Ref<VertexArray> Create();
};

DGEX_END

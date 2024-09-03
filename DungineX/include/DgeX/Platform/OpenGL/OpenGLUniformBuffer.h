#pragma once

#include "DgeX/Renderer/UniformBuffer.h"

DGEX_BEGIN

class OpenGLUniformBuffer : public UniformBuffer
{
public:
    OpenGLUniformBuffer(uint32_t size, uint32_t binding);
    ~OpenGLUniformBuffer() override;

    void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

private:
    uint32_t _rendererId = 0;
};

DGEX_END

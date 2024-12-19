#pragma once

#include "DgeX/Common/Base.h"

DGEX_BEGIN

/**
 * @brief To support multiple graphics API, we need to create a context for each API.
 */
class GraphicsContext
{
public:
    GraphicsContext() = default;
    GraphicsContext(const GraphicsContext& other) = delete;
    GraphicsContext(GraphicsContext&& other) noexcept = delete;
    GraphicsContext& operator=(const GraphicsContext& other) = delete;
    GraphicsContext& operator=(GraphicsContext&& other) noexcept = delete;
    virtual ~GraphicsContext() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static Scope<GraphicsContext> Create(void* window);
};

DGEX_END

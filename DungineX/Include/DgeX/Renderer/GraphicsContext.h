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
    GraphicsContext(const GraphicsContext&) = delete;
    GraphicsContext& operator=(const GraphicsContext&) = delete;
    GraphicsContext(GraphicsContext&&) = delete;
    GraphicsContext& operator=(GraphicsContext&&) = delete;

    virtual ~GraphicsContext() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static Scope<GraphicsContext> Create(void* window);
};

DGEX_END

/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Renderer.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 3, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Low-level renderer interface.                                              *
 ******************************************************************************/

#include "DgeX/Device/Graphics/Renderer.h"

#include "Device/Graphics/RenderCommand.h"
#include "Device/Graphics/RendererImpl.h"

#include "DgeX/Device/Graphics/Window.h"
#include "DgeX/Utils/Assert.h"

#include <algorithm>

DGEX_BEGIN

static SDL_Renderer* sNativeRenderer = nullptr;

// ============================================================================
// Concrete Renderers
// ----------------------------------------------------------------------------

void DirectRenderer::Submit(const Ref<RenderCommand>& command)
{
    command->Apply(GetNativeRenderer());
}

void DirectRenderer::Render()
{
    // Nothing.
}

void OrderedRenderer::Submit(const Ref<RenderCommand>& command)
{
    _commands.emplace_back(command);
}

void OrderedRenderer::Render()
{
    std::sort(_commands.begin(), _commands.end(), [](const Ref<RenderCommand>& lhs, const Ref<RenderCommand>& rhs) {
        return lhs->GetOrder() < rhs->GetOrder();
    });

    auto renderer = GetNativeRenderer();
    for (const auto& command : _commands)
    {
        command->Apply(renderer);
    }

    _commands.clear();
}

// ============================================================================
// API
// ----------------------------------------------------------------------------

dgex_error_t InitRenderer()
{
    int count = SDL_GetNumRenderDrivers();
    DGEX_CORE_DEBUG("Available render drivers: {0}", count);
    for (int i = 0; i < count; i++)
    {
        const char* name = SDL_GetRenderDriver(i);
        DGEX_CORE_DEBUG("    Render driver {0}: {1}", i, name);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(GetNativeWindow(), nullptr);
    if (!renderer)
    {
        DGEX_CORE_ERROR("Failed to initialize renderer: {0}", SDL_GetError());
        return DGEX_ERROR_RENDERER_INIT;
    }

    SDL_PropertiesID props = SDL_GetRendererProperties(renderer);
    const char* name = SDL_GetStringProperty(props, SDL_PROP_RENDERER_NAME_STRING, nullptr);
    DGEX_CORE_DEBUG("Using renderer: {0}", name ? name : "Unknown");

    if (!SDL_SetRenderVSync(renderer, -1))
    {
        DGEX_CORE_WARN("VSync not supported: {0}", SDL_GetError());
    }

    sNativeRenderer = renderer;

    DGEX_CORE_DEBUG("Renderer initialized");

    return DGEX_SUCCESS;
}

void DestroyRenderer()
{
    DGEX_ASSERT(sNativeRenderer, "Renderer not initialized");

    SDL_DestroyRenderer(sNativeRenderer);

    DGEX_CORE_DEBUG("Renderer destroyed");
}

SDL_Renderer* GetNativeRenderer()
{
    DGEX_ASSERT(sNativeRenderer, "Renderer not initialized");

    return sNativeRenderer;
}

Ref<Renderer> CreateRenderer(const RendererProperties& properties)
{
    DGEX_ASSERT(sNativeRenderer, "Renderer not initialized");

    if (properties.Ordered)
    {
        return CreateRef<OrderedRenderer>();
    }
    return CreateRef<DirectRenderer>();
}

DGEX_END

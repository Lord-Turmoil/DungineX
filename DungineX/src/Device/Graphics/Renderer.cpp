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
#include "Device/Graphics/RendererImpl.h"

#include "DgeX/Device/Graphics/RenderCommand.h"
#include "DgeX/Device/Graphics/Window.h"
#include "DgeX/Utils/Assert.h"

#include <algorithm>

DGEX_BEGIN

static SDL_Renderer* sNativeRenderer = nullptr;

Renderer::Renderer(SDL_Renderer* renderer) : _renderer(renderer)
{
}

// ============================================================================
// Concrete Renderers
// ----------------------------------------------------------------------------

SDL_Renderer* Renderer::GetNativeRenderer() const
{
    return _renderer;
}

DirectRenderer::DirectRenderer(SDL_Renderer* renderer) : Renderer(renderer)
{
}

void DirectRenderer::Submit(const Ref<RenderCommand>& command)
{
    command->Apply(GetNativeRenderer());
}

void DirectRenderer::SubmitImmediate(const Ref<RenderCommand>& command)
{
    Submit(command);
}

void DirectRenderer::Render()
{
    // Nothing.
}

OrderedRenderer::OrderedRenderer(SDL_Renderer* renderer) : Renderer(renderer)
{
}

void OrderedRenderer::Submit(const Ref<RenderCommand>& command)
{
    _commands.emplace_back(command);
}

void OrderedRenderer::SubmitImmediate(const Ref<RenderCommand>& command)
{
    command->Apply(GetNativeRenderer());
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
}

// ============================================================================
// API
// ----------------------------------------------------------------------------

dgex_error_t InitRenderer()
{
    SDL_Renderer* renderer = SDL_CreateRenderer(GetNativeWindow(), nullptr);
    if (!renderer)
    {
        DGEX_CORE_ERROR("Failed to initialize renderer: {0}", SDL_GetError());
        return DGEX_ERROR_RENDERER_INIT;
    }

    if (!SDL_SetRenderVSync(renderer, -1))
    {
        DGEX_CORE_WARN("VSync not supported: {0}", SDL_GetError());
    }

    sNativeRenderer = renderer;

    return DGEX_SUCCESS;
}

void DestroyRenderer()
{
    DGEX_ASSERT(sNativeRenderer, "Renderer not initialized");

    SDL_DestroyRenderer(sNativeRenderer);
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
        return CreateRef<OrderedRenderer>(sNativeRenderer);
    }
    return CreateRef<DirectRenderer>(sNativeRenderer);
}

DGEX_END

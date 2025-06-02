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
 *                    Last Update : June 2, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Low-level renderer interface.                                              *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Error.h"
#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>

DGEX_BEGIN

class Window;
class RenderCommand;

struct RendererProperties
{
    bool EnableZIndex;
};

/**
 * @brief Renderer hides details of SDL3 interface.
 *
 * There is an important matter to pay attention to, which
 * is the rendering order. So we will end up have one to
 * be aware of the order, and the other do not.
 */
class Renderer
{
public:
    explicit Renderer(SDL_Renderer* renderer);
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) noexcept = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) noexcept = delete;

    virtual ~Renderer() = default;

    SDL_Renderer* GetNativeRenderer() const;

    virtual void Submit(const Ref<RenderCommand>& command) = 0;
    virtual void Render() = 0;

private:
    // Native renderer is owned by Window, so we don't need to release it here.
    SDL_Renderer* _renderer;
};

// ============================================================================
// API
// ----------------------------------------------------------------------------

/**
 * @brief Initialize renderer context.
 *
 * @param renderer The native renderer.
 * @return 0 on success, failure otherwise.
 */
DGEX_API dgex_error_t InitRenderer(SDL_Renderer* renderer);

/**
 * @brief Initialize the renderer.
 *
 * @param properties Renderer properties.
 * @return Created renderer with the given properties.
 */
DGEX_API Ref<Renderer> CreateRenderer(const RendererProperties& properties);

DGEX_END

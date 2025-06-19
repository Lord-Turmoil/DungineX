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

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Error.h"
#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>

DGEX_BEGIN

class RenderCommand;

struct RendererProperties
{
    bool Ordered;
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
    Renderer() = default;
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) noexcept = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) noexcept = delete;

    virtual ~Renderer() = default;

    /**
     * @brief Submit a queued render command.
     *
     * @param command Render command.
     */
    virtual void Submit(const Ref<RenderCommand>& command) = 0;

    /**
     * @brief Render all commands on the target.
     */
    DGEX_API virtual void Render() = 0;
};

// ============================================================================
// API
// ----------------------------------------------------------------------------

/**
 * @brief Initialize renderer context.
 *
 * @return 0 on success, failure otherwise.
 */
dgex_error_t InitRenderer();

/**
 * @brief Destroy renderer context.
 *
 * @return 0 on success, failure otherwise.
 */
void DestroyRenderer();

/**
 * @brief Get the native renderer.
 *
 * @return The native renderer.
 */
SDL_Renderer* GetNativeRenderer();

/**
 * @brief Create a renderer.
 *
 * You can have multiple renderers at the sametime. Even though they
 * may use the same native renderer, you can have different render
 * behaviors and targets.
 *
 * @param properties Renderer properties.
 * @return Created renderer with the given properties.
 */
DGEX_API Ref<Renderer> CreateRenderer(const RendererProperties& properties);

DGEX_END

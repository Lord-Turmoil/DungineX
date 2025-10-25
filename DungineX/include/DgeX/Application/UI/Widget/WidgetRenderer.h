/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetRenderer.h                          *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 11, 2025                          *
 *                                                                            *
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Renderer for widgets. Since we use decoupled widget properties, all widgets*
 * can be rendered in a unified way.                                          *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

DGEX_BEGIN

class Texture;

namespace UI
{

struct WidgetProperties;
struct WidgetRenderContext;

class Widget;
class LabelWidget;
class FrameWidget;

/**
 * @brief Template implementation of WidgetRendererCallback.
 *
 * You have to ensure the function signature matches.
 *
 * @tparam T Type of the widget.
 * @tparam F Handler function type.
 */

/**
 * @brief A de-coupled widget renderer.
 */
class WidgetRenderer final
{
public:
    WidgetRenderer() = default;
    ~WidgetRenderer() = default;

    /**
     * @brief Render all widgets with the given root frame.
     *
     * @param widget Frame widget.
     */
    void Render(const Widget& widget) const;

    /**
     * @brief Render a widget.
     *
     * @param widget Widget to render.
     * @param context Current render context.
     */
    void Render(const Widget& widget, WidgetRenderContext& context) const;
};

} // namespace UI

DGEX_END
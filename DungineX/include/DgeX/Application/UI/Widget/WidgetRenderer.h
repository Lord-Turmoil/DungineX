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
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Renderer for widgets. Since we use decoupled widget properties, all widgets*
 * can be rendered in a unified way.                                          *
 ******************************************************************************/

#pragma once

#include "DgeX/Renderer/Color.h"
#include "DgeX/Renderer/Font.h"
#include "DgeX/Utils/Types.h"

#include <string>
#include <unordered_map>

DGEX_BEGIN

class Texture;

namespace UI
{

class Widget;
class LabelWidget;
class FrameWidget;

struct WidgetProperties;

struct WidgetRenderContext
{
    Ref<Texture> Target; // final render target, set by parent, do not modify
    Ref<Texture> Canvas; // current render target, set by current widget

    // inherited properties, can be overridden by the current widget
    Ref<FontFamily> Font;
    std::string FontStyle;
    Color FontColor;
    float FontSize;
};

/**
 * @brief Renderer callback for a specific widget type.
 */
class WidgetRendererCallback
{
public:
    WidgetRendererCallback() = default;
    virtual ~WidgetRendererCallback() = default;

    /**
     * @brief Should call _PreRender before rendering, and _PostRender after rendering.
     *
     * @param widget Widget to render.
     * @param context Rendering context.
     */
    virtual void Render(const Ref<Widget>& widget, const WidgetRenderContext& context) = 0;
};

/**
 * @brief Template implementation of WidgetRendererCallback.
 *
 * You have to ensure the function signature matches.
 *
 * @tparam T Type of the widget.
 * @tparam F Handler function type.
 */
template <typename T, typename F>
class WidgetRendererCallbackImpl : public WidgetRendererCallback
{
public:
    explicit WidgetRendererCallbackImpl(const F& callback) : _callback(callback)
    {
    }

    explicit WidgetRendererCallbackImpl(F&& callback) : _callback(std::move(callback))
    {
    }

    ~WidgetRendererCallbackImpl() override = default;

    void Render(const Ref<Widget>& widget, const WidgetRenderContext& context) override
    {
        _callback(static_cast<T&>(*widget), context);
    }

private:
    F _callback;
};

template <typename T, typename F>
DGEX_API Ref<WidgetRendererCallback> CreateWidgetRendererCallback(F&& callback)
{
    return CreateRef<WidgetRendererCallbackImpl<T, F>>(std::forward<F>(callback));
}

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
    void Render(const Ref<Widget>& widget) const;

    /**
     * @brief Render a widget.
     *
     * @param widget Widget to render.
     * @param context Current render context.
     */
    void Render(const Ref<Widget>& widget, WidgetRenderContext& context) const;

    /**
     * @brief Set the render callback for a specific widget type.
     *
     * Will override existing callback if the name already exists.
     *
     * @param name Name of the widget.
     * @param callback Render callback.
     */
    void AddCallback(const std::string& name, const Ref<WidgetRendererCallback>& callback);

private:
    std::unordered_map<std::string, Ref<WidgetRendererCallback>> _callbacks;
};

} // namespace UI

DGEX_END

/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetContext.h                           *
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
 * Context during the loading of the widget, including styles and registered  *
 * widgets. However, it does not hold resources, which are handled elsewhere  *
 * by resource manager.                                                       *
 ******************************************************************************/

#pragma once

#include "DgeX/Extension/XmlDocument.h"
#include "DgeX/Renderer/Color.h"
#include "DgeX/Renderer/Font.h"
#include "DgeX/Utils/Types.h"

#include <functional>
#include <string>
#include <unordered_map>

DGEX_BEGIN

class Texture;

namespace UI
{
class BaseWidget;
class Widget;

class Style;
class WidgetContext;

/**
 * @brief The callback to create widget.
 *
 * Return a Ref<BaseWidget> created from the context and XML element.
 * Return nullptr if creation fails.
 */
using WidgetFactoryCallback = std::function<Ref<BaseWidget>(const WidgetContext&, Ext::XmlElement)>;

struct WidgetRenderContext
{
    Ref<Texture> Target; // final render target, set by parent, do not modify
    Ref<Texture> Canvas; // current render target, set by current widget

    // inherited properties, can be overridden by the current widget
    Ref<FontFamily> Font;
    FontStyles FontStyle;
    Color FontColor;
    float FontSize;
};

/**
 * @brief Renderer callback for a specific widget type.
 */
class WidgetRenderCallback
{
public:
    WidgetRenderCallback() = default;
    virtual ~WidgetRenderCallback() = default;

    /**
     * @brief Should call _PreRender before rendering, and _PostRender after rendering.
     *
     * @param widget Widget to render.
     * @param context Rendering context.
     */
    virtual void Render(const Widget& widget, const WidgetRenderContext& context) = 0;
};

template <typename T, typename F>
class WidgetRenderCallbackImpl : public WidgetRenderCallback
{
public:
    explicit WidgetRenderCallbackImpl(const F& callback) : _callback(callback)
    {
    }

    explicit WidgetRenderCallbackImpl(F&& callback) : _callback(std::move(callback))
    {
    }

    ~WidgetRenderCallbackImpl() override = default;

    void Render(const Widget& widget, const WidgetRenderContext& context) override
    {
        _callback(static_cast<const T&>(widget), context);
    }

private:
    F _callback;
};

template <typename T, typename F>
DGEX_API Ref<WidgetRenderCallback> CreateWidgetRenderCallback(F&& callback)
{
    return CreateRef<WidgetRenderCallbackImpl<T, F>>(std::forward<F>(callback));
}

struct WidgetRegistration
{
    WidgetFactoryCallback FactoryCallback;
    Ref<WidgetRenderCallback> RenderCallback;
};

/**
 * @brief Create a widget registration with factory and render callbacks.
 *
 * Set `renderCallback` to nullptr to make this widget not renderable.
 *
 * @param factoryCallback Factory callback.
 * @param renderCallback Render callback.
 * @return Widget registration.
 */
DGEX_API Ref<WidgetRegistration> CreateWidgetRegistration(WidgetFactoryCallback factoryCallback,
                                                          Ref<WidgetRenderCallback> renderCallback);

/**
 * @brief Create a widget registration with only factory callback.
 *
 * It will use a default render callback. And the created widget must inherit `Widget`.
 *
 * @param factoryCallback Factory callback.
 * @return Widget registration.
 */
DGEX_API Ref<WidgetRegistration> CreateWidgetRegistration(WidgetFactoryCallback factoryCallback);

/**
 * @brief The context holds all styles and metadata during widget loading.
 */
class WidgetContext
{
public:
    WidgetContext() = default;

    /**
     * @brief Add a style to the context.
     *
     * @param style The style to add.
     */
    void AddStyle(const Ref<Style>& style);

    /**
     * @brief Get a style with the given name.
     *
     * @param name The name of the style.
     * @return The style of the name, or nullptr if the style does not exist.
     */
    Ref<Style> GetStyle(const std::string& name) const;

    /**
     * @brief Register a widget type.
     *
     * Overwrites any existing registration with the same name.
     *
     * @param name The name of the widget type.
     * @param registration The registration info.
     */
    void RegisterWidget(const std::string& name, Ref<WidgetRegistration> registration);

    /**
     * @brief Get the registration info of a widget type.
     *
     * @param name The name of the widget type.
     * @return The registration info.
     */
    Ref<WidgetRegistration> GetWidgetRegistration(const std::string& name) const;

private:
    // Registered styles.
    std::unordered_map<std::string, Ref<Style>> _styles;

    // Registered widgets.
    std::unordered_map<std::string, Ref<WidgetRegistration>> _registeredWidgets;
};

} // namespace UI

DGEX_END
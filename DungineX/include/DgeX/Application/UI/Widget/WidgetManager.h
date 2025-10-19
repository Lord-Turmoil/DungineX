/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetManager.h                           *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 19, 2025                          *
 *                                                                            *
 *                    Last Update : October 19, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * A manager wrapper to provide more controls over the widgets. It can allow  *
 * custom widgets to be loaded using factory pattern.                         *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/UI/Widget/Widget.h"

#include <functional>
#include <unordered_map>

DGEX_BEGIN

namespace UI
{

/**
 * @brief The callback to create widget.
 *
 * Return a Ref<BaseWidget> created from the context and XML element.
 * Return nullptr if creation fails.
 */
using WidgetFactoryCallback = std::function<Ref<BaseWidget>(const WidgetContext&, Ext::XmlElement)>;

/**
 * @brief Manage widgets and their factories.
 */
class WidgetManager final
{
public:
    /**
     * @brief Load widgets from XML element.
     *
     * The context is provided by the application, who holds all context data.
     *
     * @param context Widget context.
     * @param element Root widget element.
     * @return true if succeeded, false otherwise.
     */
    bool Load(const WidgetContext& context, Ext::XmlElement element);

    /**
     * @brief Get the root widget.
     *
     * Note that this returns nullptr if not loaded or load failed.
     *
     * @return The root widget.
     */
    Ref<Widget> GetRootWidget() const;

    /**
     * @brief Register a widget factory callback for a specific widget type.
     *
     * Will override existing callback if the name already exists.
     *
     * @param name Name of the widget, indicated by the XML tag name.
     * @param callback The factory callback.
     */
    void RegisterWidget(const std::string& name, const WidgetFactoryCallback& callback);

public:
    void Update(DeltaTime delta) const;
    void OnEvent(const Ref<Event>& event) const;

    /**
     * @brief Manually reorder widgets.
     */
    void Reorder();

private:
    Ref<BaseWidget> _Load(const WidgetContext& context, Ext::XmlElement element);

private:
    /**
     * @brief The root widget should be a frame.
     */
    Ref<Widget> _root;

    /**
     * @brief All widgets sorted by their order to receive events.
     */
    std::vector<Ref<BaseWidget>> _widgets;

    std::unordered_map<std::string, WidgetFactoryCallback> _callbacks;
};

} // namespace UI

DGEX_END

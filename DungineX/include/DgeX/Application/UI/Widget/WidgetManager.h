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
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * A manager wrapper to provide more controls over the widgets. It can allow  *
 * custom widgets to be loaded using factory pattern.                         *
 ******************************************************************************/

#pragma once

#include "DgeX/Core/Timer.h"
#include "DgeX/Extension/XmlDocument.h"
#include "DgeX/Utils/Types.h"

#include <vector>

DGEX_BEGIN

class Event;

namespace UI
{

class BaseWidget;
class Widget;
class WidgetContext;

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

public:
    void Update(DeltaTime delta) const;
    void OnEvent(Event& event) const;

private:
    Ref<BaseWidget> _Load(const WidgetContext& context, Ext::XmlElement element);

private:
    /**
     * @brief The root widget should be a frame.
     */
    Ref<Widget> _root;
};

} // namespace UI

DGEX_END
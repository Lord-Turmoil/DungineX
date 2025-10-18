/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : BaseWidget.h                              *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 4, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base widget element definition.                                            *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/Event/Event.h"
#include "DgeX/Application/UI/Style/Style.h"
#include "DgeX/Core/Timer.h"
#include "DgeX/Extension/XmlDocument.h"
#include "DgeX/Utils/Types.h"

#include <string>
#include <unordered_map>
#include <vector>

DGEX_BEGIN

class Event;

namespace UI
{

class WidgetContext;
class Style;

class Widget;

class BaseWidget
{
public:
    BaseWidget();
    BaseWidget(std::string name, std::string id);
    BaseWidget(const WidgetContext& context, Ext::XmlElement element);

    virtual ~BaseWidget() = default;

    /**
     * @brief Get the name of the widget.
     *
     * The name is usually indicated as the XML element name.
     *
     * @return The name of the widget.
     */
    DGEX_API const std::string& GetName() const;

    /**
     * @brief Get the ID of the widget.
     *
     * @return The widget's ID.
     */
    DGEX_API const std::string& GetId() const;

    /**
     * @brief Get the state of the widget.
     *
     * @return The state of the widget.
     */
    DGEX_API StyleState GetState() const;

    /**
     * @brief Get the string value of the state.
     *
     * @return The string value of the state.
     */
    DGEX_API std::string GetStateValue() const;

    DGEX_API void SetState(StyleState state);

    DGEX_API bool IsNormal() const;
    DGEX_API bool IsHover() const;
    DGEX_API bool IsActive() const;
    DGEX_API bool IsDisabled() const;

    /**
     * @brief Get the style of the widget.
     *
     * @return The style of the widget.
     */
    DGEX_API Ref<Style> GetStyle() const;

    template <typename T>
    DGEX_API T GetStyleProperty(const std::string& name, const T& defaultValue = T()) const;

    /**
     * @brief Check if the style has a specific property.
     *
     * This checks the property in the current state first, then falls back to
     * the base properties.
     *
     * @param name The name of the property.
     * @return Whether the property exists or not.
     */
    DGEX_API bool HasStyleProperty(const std::string& name) const;

    /**
     * @brief Get the parent widget.
     *
     * The parent widget may be null if this widget has no parent, or somehow the
     * parent is destroyed, and you still hold an orphaned child widget.
     *
     * @return The parent widget.
     */
    DGEX_API Ref<BaseWidget> Parent() const;

    /**
     * @brief Get all children widgets.
     *
     * @return All children widgets.
     */
    DGEX_API const std::vector<Ref<BaseWidget>>& Children() const;

    /**
     * @brief Add a direct child widget.
     *
     * This will add the widget to the end of the children list. Currently we are
     * not sensitive to the ordering. And if the widget already has a parent, it will
     * be removed from the previous parent first.
     *
     * @param widget The child widget to add.
     */
    DGEX_API void AddChild(const Ref<BaseWidget>& widget);

    /**
     * @brief Remove a direct child from this widget.
     *
     * @param widget The widget to remove.
     */
    DGEX_API void RemoveChild(const Ref<BaseWidget>& widget);

    /**
     * @brief Clear all children widgets.
     */
    DGEX_API void ClearChildren();

    /*
     * Get a child widget by its ID.
     *
     * This will recursively search through all descendants.
     */
    DGEX_API Ref<BaseWidget> GetChildById(const std::string& id) const;

    DGEX_API virtual Ref<Widget> AsWidget();

public:
    /**
     * @brief Update the widget.
     *
     * @param delta Delta time elapsed since last update.
     */
    virtual void Update(DeltaTime delta);

    /**
     * @brief Handling event.
     *
     * The event is dispatched immediately to the widget and its listeners.
     * This should be called be every inherited widget class to ensure the
     * event is propagated correctly.
     *
     * @param event On receiving event.
     */
    virtual void OnEvent(const Ref<Event>& event);

    /**
     * @brief Add an event listener to the widget.
     *
     * @param type Event type to listen to.
     * @param listener The event listener.
     * @return Whether the listener is added successfully or not.
     */
    DGEX_API bool AddEventListener(EventType type, const Ref<EventListener>& listener);

    /**
     * @brief Remove a specific event listener from the widget.
     *
     * @param type The event type.
     * @param listener The event listener to remove.
     * @return Whether the listener is removed successfully or not.
     */
    DGEX_API void RemoveEventListener(EventType type, const Ref<EventListener>& listener);

    /**
     * @brief Remove all event listeners of a specific type from the widget.
     *
     * @param type The event type.
     */
    DGEX_API void RemoveEventListeners(EventType type);

protected:
    /**
     * @brief Notify all listeners of an event.
     *
     * @param event Event to send.
     */
    void _Notify(const Ref<Event>& event);

    /**
     * @brief Check if a position is inside the widget.
     *
     * @param position The position to check.
     * @return Whether the position is inside the widget or not.
     */
    virtual bool _IsInside(FPoint position) const;

    /**
     * @brief Update properties when the style changes.
     *
     * This is most likely due to state change.
     */
    virtual void ApplyStyles();

private:
    void _OnEventNormal(const Ref<Event>& event);
    void _OnEventHover(const Ref<Event>& event);
    void _OnEventActive(const Ref<Event>& event);
    void _OnEventDisabled(const Ref<Event>& event);

private:
    std::string _name; // The name of the widget, usually the XML element name.
    std::string _id;   // The unique ID of the widget.
    StyleState _state;
    Ref<Style> _style;

    std::unordered_map<EventType, std::vector<Ref<EventListener>>> _listeners;

protected:
    WeakRef<BaseWidget> _parent;
    std::vector<Ref<BaseWidget>> _children;
};

template <typename T>
T BaseWidget::GetStyleProperty(const std::string& name, const T& defaultValue) const
{
    if (_state == StyleState::Normal)
    {
        return _style->GetPropertyAs(name, defaultValue);
    }
    return _style->GetStatePropertyAs(_state, name, defaultValue);
}

} // namespace UI

DGEX_END

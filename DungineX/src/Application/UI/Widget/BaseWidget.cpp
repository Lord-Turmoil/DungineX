/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : BaseWidget.cpp                            *
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

#include "DgeX/Application/UI/Widget/BaseWidget.h"

#include "DgeX/Application/Event/MouseEvents.h"
#include "DgeX/Application/Event/WidgetEvents.h"
#include "DgeX/Application/UI/Widget/WidgetContext.h"
#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Macros.h"
#include "DgeX/Utils/UUID.h"

#include <algorithm>

DGEX_BEGIN

namespace UI
{

BaseWidget::BaseWidget() : _id(UUID().ToString()), _state(StyleState::Normal)
{
}

BaseWidget::BaseWidget(std::string name, std::string id)
    : _name(std::move(name)), _id(std::move(id)), _state(StyleState::Normal)
{
}

BaseWidget::BaseWidget(const WidgetContext& context, Ext::XmlElement element)
    : _id(UUID().ToString()), _state(StyleState::Normal)
{
    DGEX_ASSERT(element.IsValid(), "Invalid XML for widget construction");

    _name = element.Name();

    // Ensure all widgets have an ID.
    if (const char* id = element.Attribute("id"))
    {
        _id = id;
    }
    else
    {
        _id = UUID().ToString();
    }

    // Load styles.
    const char* style = element.Attribute("style");
    Ref<Style> baseStyle;
    if (style)
    {
        baseStyle = context.GetStyle(style);
    }

    if (baseStyle)
    {
        _style = CreateRef<Style>(*baseStyle);
        _style->Merge(element);
    }
    else
    {
        _style = CreateRef<Style>(element);
    }
}

const std::string& BaseWidget::GetName() const
{
    return _name;
}

const std::string& BaseWidget::GetId() const
{
    return _id;
}

StyleState BaseWidget::GetState() const
{
    return _state;
}

std::string BaseWidget::GetStateValue() const
{
    return ToString(_state);
}

void BaseWidget::SetState(StyleState state)
{
    if (_state != state)
    {
        _state = state;
        ApplyStyles();
    }
}

bool BaseWidget::IsNormal() const
{
    return _state == StyleState::Normal;
}

bool BaseWidget::IsHover() const
{
    return _state == StyleState::Hover;
}

bool BaseWidget::IsActive() const
{
    return _state == StyleState::Active;
}

bool BaseWidget::IsDisabled() const
{
    return _state == StyleState::Disabled;
}

Ref<Style> BaseWidget::GetStyle() const
{
    return _style;
}

bool BaseWidget::HasStyleProperty(const std::string& name) const
{
    if (_state == StyleState::Normal)
    {
        return _style->HasProperty(name);
    }

    if (_style->HasStateProperty(_state, name))
    {
        return true;
    }

    return _style->HasProperty(name);
}

Ref<BaseWidget> BaseWidget::Parent() const
{
    return _parent.lock();
}

const std::vector<Ref<BaseWidget>>& BaseWidget::Children() const
{
    return _children;
}

void BaseWidget::AddChild(const Ref<BaseWidget>& widget)
{
    DGEX_ASSERT(widget, "Widget to add cannot be null");

    if (widget->Parent())
    {
        DGEX_CORE_WARN("Widget {} already has a parent, removing from the previous parent", widget->GetId());
        widget->Parent()->RemoveChild(widget);
    }
    _children.push_back(widget);
}

void BaseWidget::RemoveChild(const Ref<BaseWidget>& widget)
{
    DGEX_ASSERT(widget, "Widget to remove cannot be null");

    for (auto it = _children.begin(); it != _children.end(); ++it)
    {
        if (*it == widget)
        {
            _children.erase(it);
            widget->_parent.reset();
            return;
        }
    }
}

void BaseWidget::ClearChildren()
{
    for (auto& child : _children)
    {
        child->_parent.reset();
    }
    _children.clear();
}

Ref<BaseWidget> BaseWidget::GetChildById(const std::string& id) const
{
    for (const auto& child : _children)
    {
        if (child->GetId() == id)
        {
            return child;
        }
    }

    for (const auto& child : _children)
    {
        if (auto descendant = child->GetChildById(id))
        {
            return descendant;
        }
    }

    return nullptr;
}

Ref<Widget> BaseWidget::AsWidget()
{
    return nullptr;
}

void BaseWidget::Update(DeltaTime delta)
{
    DGEX_USED(delta);
}

void BaseWidget::OnEvent(const Ref<Event>& event)
{
    if (!event->IsHandled())
    {
        _Notify(event);
    }

    if (!event->IsHandled())
    {
        switch (_state)
        {
        case StyleState::Normal:
            _OnEventNormal(event);
            break;
        case StyleState::Hover:
            _OnEventHover(event);
            break;
        case StyleState::Active:
            _OnEventActive(event);
            break;
        case StyleState::Disabled:
            _OnEventDisabled(event);
            break;
        case StyleState::NumStates:
            break;
        }
    }

    if (!event->IsHandled())
    {
        if (auto parent = Parent())
        {
            parent->OnEvent(event);
        }
    }
}

bool BaseWidget::AddEventListener(EventType type, const Ref<EventListener>& listener)
{
    DGEX_ASSERT(listener, "Event listener cannot be null");

    if (std::find(_listeners[type].begin(), _listeners[type].end(), listener) != _listeners[type].end())
    {
        DGEX_LOG_WARN("Listener already exists for event {} in widget {}", ToString(type), _id);
        return false;
    }
    _listeners[type].push_back(listener);

    return true;
}

void BaseWidget::RemoveEventListener(EventType type, const Ref<EventListener>& listener)
{
    if (!listener)
    {
        RemoveEventListeners(type);
        return;
    }

    _listeners[type].erase(std::remove(_listeners[type].begin(), _listeners[type].end(), listener),
                           _listeners[type].end());
}

void BaseWidget::RemoveEventListeners(EventType type)
{
    _listeners[type].clear();
}

void BaseWidget::_Notify(const Ref<Event>& event)
{
    for (const auto& listener : _listeners[event->GetType()])
    {
        if (listener->OnEvent(event))
        {
            event->SetHandled(true);
        }

        if (event->IsHandled())
        {
            break;
        }
    }
}

bool BaseWidget::_IsInside(FPoint position) const
{
    DGEX_USED(position);
    return false;
}

void BaseWidget::ApplyStyles()
{
}

void BaseWidget::_OnEventNormal(const Ref<Event>& event)
{
    DispatchEvent<MouseMovedEvent>(event, [this](MouseMovedEvent& e) {
        if (_IsInside(e.GetPosition()))
        {
            SetState(StyleState::Hover);
            _Notify(CreateRef<MouseEnterEvent>());
        }
        return true; // prevent propagation
    });
}

void BaseWidget::_OnEventHover(const Ref<Event>& event)
{
    DispatchEvent<MouseMovedEvent>(event, [this](MouseMovedEvent& e) {
        if (!_IsInside(e.GetPosition()))
        {
            SetState(StyleState::Normal);
            _Notify(CreateRef<MouseLeaveEvent>());
        }
        return true; // prevent propagation
    });

    DispatchEvent<MouseButtonPressedEvent>(event, [this](MouseButtonPressedEvent& e) {
        SetState(StyleState::Active);
        DGEX_USED(e);
        return true; // prevent propagation
    });
}

void BaseWidget::_OnEventActive(const Ref<Event>& event)
{
    DispatchEvent<MouseMovedEvent>(event, [this](MouseMovedEvent& e) {
        if (!_IsInside(e.GetPosition()))
        {
            SetState(StyleState::Normal);
            _Notify(CreateRef<MouseLeaveEvent>());
        }
        return true; // prevent propagation
    });

    DispatchEvent<MouseButtonReleasedEvent>(event, [this](MouseButtonReleasedEvent& e) {
        SetState(StyleState::Hover);
        _Notify(CreateRef<MouseClickEvent>());
        DGEX_USED(e);
        return true; // prevent propagation
    });
}

void BaseWidget::_OnEventDisabled(const Ref<Event>& event)
{
    DGEX_USED(event);
}

} // namespace UI

DGEX_END

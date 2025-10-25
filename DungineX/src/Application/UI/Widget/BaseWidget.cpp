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
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base widget element definition.                                            *
 * -------------------------------------------------------------------------- *
 * REFERENCES:                                                                *
 *                                                                            *
 * The reaction of the widget to events follows that of HTML elements. Even   *
 * if two elements overlap, all elements can receive mouse events. But the    *
 * current implementation does not guarantee the order of elements receiving  *
 * the events.                                                                *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/BaseWidget.h"

#include "DgeX/Application/Event/MouseEvents.h"
#include "DgeX/Application/Event/WidgetEvents.h"
#include "DgeX/Application/UI/Style/WidgetProperty.h"
#include "DgeX/Application/UI/Widget/WidgetContext.h"
#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Macros.h"
#include "DgeX/Utils/UUID.h"

#include <algorithm>

DGEX_BEGIN

namespace UI
{

BaseWidget::BaseWidget()
    : _id(UUID().ToString()), _state(StyleState::Normal), _style(CreateRef<Style>()), _level(0), _zIndex(0),
      _hold(false)
{
}

BaseWidget::BaseWidget(std::string name, std::string id)
    : _name(std::move(name)), _id(std::move(id)), _state(StyleState::Normal), _style(CreateRef<Style>()), _level(0),
      _zIndex(0), _hold(false)
{
}

BaseWidget::BaseWidget(const WidgetContext& context, Ext::XmlElement element)
    : _id(UUID().ToString()), _state(StyleState::Normal), _style(CreateRef<Style>()), _level(0), _zIndex(0),
      _hold(false)
{
    DGEX_ASSERT(element.IsValid(), "Invalid XML for widget construction");

    _name = element.Name();
    _id = element.AttributeAs<StringProperty>("id", StringProperty(UUID().ToString())).Value;
    _zIndex = element.AttributeAs<IntegerProperty>("z-index", IntegerProperty(0)).Value;

    // Load styles.
    if (const char* style = element.Attribute("style"))
    {
        if (Ref<Style> baseStyle = context.GetStyle(style))
        {
            _style->Merge(baseStyle);
        }
        else
        {
            DGEX_CORE_WARN("Missing base style {}", style);
        }
    }
    _style->Merge(element);
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
    widget->_SetParent(shared_from_this());

    // Ensure children are ordered by z-index from low to high.
    const auto it = std::find_if(_children.begin(), _children.end(), [widget](const Ref<BaseWidget>& child) {
        return child->GetZIndex() > widget->GetZIndex();
    });
    _children.insert(it, widget);
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

int BaseWidget::GetLevel() const
{
    return _level;
}

int BaseWidget::GetZIndex() const
{
    return _zIndex;
}

Ptr<Widget> BaseWidget::AsWidget()
{
    return nullptr;
}

Ref<Widget> BaseWidget::AsWidgetRef()
{
    return nullptr;
}

Ptr<FrameWidget> BaseWidget::AsFrameWidget()
{
    return nullptr;
}

Ref<FrameWidget> BaseWidget::AsFrameWidgetRef()
{
    return nullptr;
}

void BaseWidget::Update(DeltaTime delta)
{
    DGEX_USED(delta);
}

void BaseWidget::OnEvent(Event& event)
{
    if (!event.IsHandled())
    {
        _Notify(event);
    }

    if (!event.IsHandled())
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

void BaseWidget::_SetState(StyleState state)
{
    if (_state != state)
    {
        _state = state;
        if (state == StyleState::Active)
        {
            _hold = true;
        }
        ApplyStyles();
    }
}

void BaseWidget::_SetParent(const Ref<BaseWidget>& parent)
{
    _parent = parent;

    _level = 0;
    Ref<BaseWidget> p = parent;
    while (p)
    {
        _level++;
        p = p->Parent();
    }
}

void BaseWidget::_Notify(Event& event)
{
    for (const auto& listener : _listeners[event.GetType()])
    {
        if (listener->OnEvent(event))
        {
            event.SetHandled(true);
        }

        if (event.IsHandled())
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

void BaseWidget::_OnEventNormal(Event& event)
{
    DispatchEvent<MouseMovedEvent>(event, [this](const MouseMovedEvent& e) {
        if (_IsInside(e.GetPosition()))
        {
            _SetState(StyleState::Hover);
            auto event = MouseEnterEvent();
            _Notify(event);
        }
        return false; // allow propagation
    });
}

void BaseWidget::_OnEventHover(Event& event)
{
    DispatchEvent<MouseMovedEvent>(event, [this](const MouseMovedEvent& e) {
        if (!_IsInside(e.GetPosition()))
        {
            _SetState(StyleState::Normal);
            MouseLeaveEvent mouseLeaveEvent;
            _Notify(mouseLeaveEvent);
        }
        return false; // allow propagation
    });

    DispatchEvent<MouseButtonPressedEvent>(event, [this](const MouseButtonPressedEvent& e) {
        _SetState(StyleState::Active);
        DGEX_USED(e);
        return false; // allow propagation
    });
}

void BaseWidget::_OnEventActive(Event& event)
{
    DispatchEvent<MouseMovedEvent>(event, [this](const MouseMovedEvent& e) {
        if (_IsInside(e.GetPosition()))
        {
            if (!_hold)
            {
                MouseEnterEvent mouseEnterEvent;
                _Notify(mouseEnterEvent);
                _hold = true;
            }
        }
        else
        {
            if (_hold)
            {
                MouseLeaveEvent mouseLeaveEvent;
                _Notify(mouseLeaveEvent);
                _hold = false;
            }
        }
        return false; // allow propagation
    });

    DispatchEvent<MouseButtonReleasedEvent>(event, [this](const MouseButtonReleasedEvent& e) {
        DGEX_USED(e);
        if (_hold)
        {
            _SetState(StyleState::Hover);
            MouseClickedEvent mouseClickedEvent;
            _Notify(mouseClickedEvent);
        }
        else
        {
            _SetState(StyleState::Normal);
        }
        return false; // prevent propagation
    });
}

void BaseWidget::_OnEventDisabled(Event& event)
{
    DGEX_USED(event);
}

} // namespace UI

DGEX_END
#pragma once

#include "DgeX/Core/Base.h"

#include <functional>
#include <string>

DGEX_BEGIN

// clang-format off
enum class EventType : unsigned char
{
    None,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    InterfaceTransit, InterfaceChange, InterfaceClose,
    KeyPressed, KeyReleased, KeyTyped,
    MouseMoved, MouseScrolled,
    MouseButtonPressed, MouseButtonReleased, MouseButtonClicked
};

// clang-format off
enum EventCategory : unsigned char
{
    None = 0,
    EventCategoryApplication  = BIT(0),
    EventCategoryInterface    = BIT(1),
    EventCategoryInput        = BIT(2),
    EventCategoryKeyboard     = BIT(3),
    EventCategoryMouse        = BIT(4),
    EventCategoryMouseButton  = BIT(5)
};

// clang-format on

#define DECL_EVENT_CLASS_TYPE(type)                                                                                    \
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    virtual EventType GetEventType() const override                                                                    \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const char* GetName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

#define DECL_EVENT_CLASS_CATEGORY(category)                                                                            \
    virtual int GetCategoryFlags() const override                                                                      \
    {                                                                                                                  \
        return category;                                                                                               \
    }

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;

    virtual std::string ToString() const
    {
        return GetName();
    }

    bool IsInCategory(EventCategory category) const
    {
        return GetCategoryFlags() & category;
    }

    // C# style public member LOL
    bool Handled = false;
};

/**
 * @brief Primitive function to dispatch an event.
 * @tparam T Event type.
 * @tparam F Event handler function, will be deducted by the compiler, takes a T&.
 * @param event Event to dispatch.
 * @param func Handler function.
 * @return Whether the event is handled or not.
 */
template <typename T, typename F> bool DispatchEvent(const Ref<Event>& event, const F& func)
{
    if (event->GetEventType() == T::GetStaticType())
    {
        event->Handled |= func(static_cast<T&>(*event));
        return true;
    }
    return false;
}

/**
 * @brief This is a helpful class to dispatch events with specific type.
 */
class EventDispatcher
{
public:
    explicit EventDispatcher(const Ref<Event>& event) : _event(event)
    {
    }

    // F will be deduced by the compiler
    template <typename T, typename F> bool Dispatch(const F& func)
    {
        return DispatchEvent<T>(_event, func);
    }

private:
    Ref<Event> _event;
};

using EventCallbackFn = std::function<void(const Ref<Event>&)>;

DGEX_END

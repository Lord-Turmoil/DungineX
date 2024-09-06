#pragma once

#include "DgeX/Common/Base.h"

DGEX_BEGIN

enum class EventType : unsigned char
{
    None,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory : unsigned char
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define DECL_EVENT_CLASS_TYPE(type)                                                                                    \
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    virtual EventType GetEventType() const override                                                                    \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const wchar_t *GetName() const override                                                                    \
    {                                                                                                                  \
        return L#type;                                                                                                 \
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
    virtual const wchar_t *GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;

    virtual std::wstring ToString() const
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
 * @brief This is a helpful class to dispatch events with specific type.
 */
class EventDispatcher
{
public:
    explicit EventDispatcher(const Ref<Event> &event) : _event(event)
    {
    }

    // F will be deduced by the compiler
    template <typename T, typename F> bool Dispatch(const F &func)
    {
        if (_event->GetEventType() == T::GetStaticType())
        {
            _event->Handled |= func(static_cast<T &>(_event));
            return true;
        }
        return false;
    }

private:
    Ref<Event> _event;
};

DGEX_END

#pragma once

#include "DgeX/Application/Event/Event.h"

DGEX_BEGIN

class DomElement;

template <typename TEvent> using EventListenerCallback = std::function<bool(Ptr<DomElement>, const TEvent&)>;

class EventListener
{
public:
    template <typename TEvent>
    EventListener(Ptr<DomElement> element, const EventListenerCallback<TEvent>& callback)
        : _delegate(CreateRef<EventDelegate<TEvent>>(std::bind(callback, element, std::placeholders::_1)))
    {
    }

    void OnEvent(const Ref<Event>& event) const
    {
        _delegate->Dispatch(event);
    }

private:
    template <typename TEvent> using EventDelegateCallback = std::function<bool(const TEvent&)>;

    class AbstractEventDelegate
    {
    public:
        virtual ~AbstractEventDelegate() = default;
        virtual void Dispatch(const Ref<Event>& event) = 0;
    };

    template <typename TEvent> class EventDelegate : public AbstractEventDelegate
    {
    public:
        template <typename U> EventDelegate(const U& callback) : _callback(callback)
        {
        }

        ~EventDelegate() override = default;

        void Dispatch(const Ref<Event>& event) override
        {
            _callback(*std::static_pointer_cast<TEvent>(event));
        }

    private:
        EventDelegateCallback<TEvent> _callback;
    };

private:
    Ref<AbstractEventDelegate> _delegate;
};

DGEX_END

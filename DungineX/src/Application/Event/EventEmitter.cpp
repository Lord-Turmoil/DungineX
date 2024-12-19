#include "DgeX/Application/Event/EventEmitter.h"

DGEX_BEGIN

EventCallbackFn EventEmitter::_sEventCallback = nullptr;

void EventEmitter::SetEventCallBack(const EventCallbackFn& callback)
{
    _sEventCallback = callback;
}

void EventEmitter::Emit(const Ref<Event>& event)
{
    if (_sEventCallback)
    {
        _sEventCallback(event);
    }
}

DGEX_END

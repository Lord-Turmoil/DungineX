#pragma once

#include "DgeX/Application/Event/Event.h"

DGEX_BEGIN

class EventEmitter
{
public:
    static void SetEventCallBack(const EventCallbackFn& callback);
    static void Emit(const Ref<Event>& event);

private:
    static EventCallbackFn _sEventCallback;
};

DGEX_END

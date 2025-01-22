#include "DgeX/Application/Event/EventBuffer.h"

DGEX_BEGIN

void EventBuffer::PushEvent(const Ref<Event>& event)
{
    std::lock_guard lock(_mutex);
    _backBuffer.emplace_back(event);
}

void EventBuffer::SwapBuffer()
{
    std::lock_guard lock(_mutex);
    _frontBuffer.swap(_backBuffer);
    _backBuffer.clear();
}

void EventBuffer::Clear()
{
    std::lock_guard lock(_mutex);
    _frontBuffer.clear();
    _backBuffer.clear();
}

DGEX_END

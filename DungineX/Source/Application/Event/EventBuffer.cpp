#include "DgeX/Application/Event/EventBuffer.h"

DGEX_BEGIN

void EventBuffer::PushEvent(const Ref<Event>& event)
{
    _backBuffer.emplace_back(event);
}

void EventBuffer::SwapBuffer()
{
    _frontBuffer.swap(_backBuffer);

    // There is little chance that new events are pushed during the swap
    // in which case clear will accidentally delete them.
    _backBuffer.clear();
}

DGEX_END

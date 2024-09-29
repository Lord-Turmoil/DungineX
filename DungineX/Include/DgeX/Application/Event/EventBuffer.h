#pragma once

#include "DgeX/Application/Event/Event.h"

#include <mutex>

DGEX_BEGIN

// clang-format off
class EventBuffer
{
public:
    EventBuffer() = default;

    void PushEvent(const Ref<Event>& event);
    void SwapBuffer();
    void Clear();
    
	std::vector<Ref<Event>>::iterator begin() { return _frontBuffer.begin(); }
	std::vector<Ref<Event>>::iterator end() { return _frontBuffer.end(); }
	std::vector<Ref<Event>>::reverse_iterator rbegin() { return _frontBuffer.rbegin(); }
	std::vector<Ref<Event>>::reverse_iterator rend() { return _frontBuffer.rend(); }

	std::vector<Ref<Event>>::const_iterator begin() const { return _frontBuffer.begin(); }
	std::vector<Ref<Event>>::const_iterator end()	const { return _frontBuffer.end(); }
	std::vector<Ref<Event>>::const_reverse_iterator rbegin() const { return _frontBuffer.rbegin(); }
	std::vector<Ref<Event>>::const_reverse_iterator rend() const { return _frontBuffer.rend(); }

private:
    std::vector<Ref<Event>> _frontBuffer;
    std::vector<Ref<Event>> _backBuffer;
    std::mutex _mutex;  // for thread safety and race condition
};

// clang-format on

DGEX_END

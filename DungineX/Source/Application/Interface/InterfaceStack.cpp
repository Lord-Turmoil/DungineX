#include "DgeX/Application/Interface/InterfaceStack.h"
#include "DgeX/Application/Interface/Interface.h"

DGEX_BEGIN

bool InterfaceStack::Push(Interface* interface)
{
    _interfaces.push_back(interface);
    return _loaded.insert(interface->GetName()).second;
}

void InterfaceStack::Pop()
{
    DGEX_ASSERT(!_interfaces.empty(), "No interface to pop");

    if (!_interfaces.empty())
    {
        _interfaces.pop_back();
    }
}

void InterfaceStack::Clear()
{
    _interfaces.clear();
    _loaded.clear();
}

Interface* InterfaceStack::Current() const
{
    if (!_interfaces.empty())
    {
        return _interfaces.back();
    }

    return nullptr;
}

DGEX_END

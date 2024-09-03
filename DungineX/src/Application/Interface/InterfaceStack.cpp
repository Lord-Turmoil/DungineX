#include "DgeX/Application/Interface/InterfaceStack.h"

#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Core/Assert.h"

DGEX_BEGIN

void InterfaceStack::Push(Interface* interface)
{
    _interfaces.push_back(interface);
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

#include "DgeX/Application/Interface/InterfaceStack.h"
#include "DgeX/Application/Interface/Interface.h"

DGEX_BEGIN

bool InterfaceStack::PushInterface(Interface* interface)
{
    _interfaces.push_back(interface);
    return _loaded.insert(interface->GetName()).second;
}

void InterfaceStack::PopInterface()
{
    DGEX_ASSERT(!_interfaces.empty(), "No interface to pop");

    if (!_interfaces.empty())
    {
        _interfaces.pop_back();
    }
}

void InterfaceStack::ClearInterfaces()
{
    _interfaces.clear();
    _loaded.clear();
}

Interface* InterfaceStack::CurrentInterface() const
{
    if (!_interfaces.empty())
    {
        return _interfaces.back();
    }

    return nullptr;
}

DGEX_END

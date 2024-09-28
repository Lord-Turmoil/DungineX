#pragma once

#include "DgeX/Common/Common.h"

#include <unordered_set>

DGEX_BEGIN

class Interface;

class InterfaceStack
{
public:
    InterfaceStack() = default;

    /**
     * @brief Push an interface to the stack.
     * @param interface the interface to add
     * @return whether is the first time seeing this interface
     * @note return value is used to determine whether to call OnLoad()
     *      the return value will be false if interface is popped out, then pushed back.
     *      However, it will be reset if ClearInterfaces() is called.
     *
     */
    bool PushInterface(Interface* interface);
    void PopInterface();
    void ClearInterfaces();

    Interface* CurrentInterface() const;

private:
    std::vector<Interface*> _interfaces;
    std::unordered_set<std::string> _loaded;
};

DGEX_END

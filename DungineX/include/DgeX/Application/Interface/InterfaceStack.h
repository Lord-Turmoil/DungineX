#pragma once

#include "DgeX/Core/Macros.h"

#include <string>
#include <unordered_set>
#include <vector>

DGEX_BEGIN

class Interface;

class InterfaceStack
{
public:
    InterfaceStack() = default;
    InterfaceStack(const InterfaceStack& other) = delete;
    InterfaceStack(InterfaceStack&& other) noexcept = delete;
    InterfaceStack& operator=(const InterfaceStack& other) = delete;
    InterfaceStack& operator=(InterfaceStack&& other) noexcept = delete;

    /**
     * @brief Push an interface to the stack.
     * @param interface the interface to add
     * @return whether is the first time seeing this interface
     * @note return value is used to determine whether to call OnLoad()
     *      the return value will be false if interface is popped out, then pushed back.
     *      However, it will be reset if ClearInterfaces() is called.
     *
     */
    void Push(Interface* interface);
    void Pop();
    void Clear();

    /**
     * @brief Get the current active interface, i.e. the top interface.
     * @return The current interface. nullptr if no interface presents.
     */
    Interface* Current() const;

private:
    std::vector<Interface*> _interfaces;
};

DGEX_END

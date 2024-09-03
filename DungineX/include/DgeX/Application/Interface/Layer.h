#pragma once

#include "DgeX/Core/Base.h"
#include "DgeX/Core/DeltaTime.h"

#include <string>

DGEX_BEGIN

class Event;

/**
 * @brief Layer represents a layer of the interface.
 * @paragraph By dividing UI elements into layers, we can manage the rendering
 *      order and layout easier.
 * @note Usually, the base layer or the interface itself handles the update
 *      logic and events in the interface.
 * @todo Widgets are managed by the layer.
 */
// clang-format off
class Layer
{
    friend class Interface;

public:
    Layer(const std::string& name = "");
    virtual ~Layer() = default;

    const std::string& GetName() const { return _name; }
    int GetWidth() const;
    int GetHeight() const;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(DeltaTime /*delta*/) {}
    virtual void OnRender() {}
    virtual void OnEvent(const Ref<Event>& /*event*/) {}

private:
    /**
     * @brief Set parent interface for the layer.
     * @note Remember to set to nullptr when the layer is detached.
     * @param parent parent interface
     */
    void SetParent(Interface* parent) { _parent = parent; }

protected:
    std::string _name;
    Interface* _parent;
};

// clang-format on

DGEX_END

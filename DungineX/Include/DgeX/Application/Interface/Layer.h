#pragma once

#include "DgeX/Application/Event/Event.h"
#include "DgeX/Common/Base.h"
#include "DgeX/Core/DeltaTime.h"

DGEX_BEGIN

// clang-format off
/**
 * @brief Layer represents a layer of the interface.
 * @paragraph By dividing UI elements into layers, we can manage the rendering
 *      order and layout easier.
 * @note Usually, the base layer handles the update logic and events in the
 *      interface.
 */
class Layer
{
    friend class Interface;
public:
    Layer(const std::string& name = "");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(DeltaTime delta) {}
    virtual void OnRender() {}
    virtual void OnEvent(const Ref<Event>&event) {}

    const std::string& GetName() const { return _name; }

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

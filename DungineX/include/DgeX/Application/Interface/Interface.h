#pragma once

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Interface/LayerStack.h"
#include "DgeX/Core/DeltaTime.h"

DGEX_BEGIN

class Layer;
class InterfaceCamera;

/**
 * @brief Interface is a collection of layers, so that we can manage interface
 *      changes and layer stack easily.
 * @note Generally, you should override the following functions:
 *          OnLoad       -- when interface added to application
 *          OnMounted    -- every time when switch to this interface
 *          OnUnmounted  -- every time when switch from this interface
 *          OnUnload     -- when interface removed from application
 */
// clang-format off
class Interface
{
    friend class Application;

public:
    Interface(const std::string& name = "");
    Interface(const Interface& other) = delete;
    Interface(Interface&& other) noexcept = delete;
    Interface& operator=(const Interface& other) = delete;
    Interface& operator=(Interface&& other) noexcept = delete;

    virtual ~Interface() = default;

    const std::string& GetName() const { return _name; }
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    bool IsLoaded() const { return _loaded; }

    /**
     * @brief Called when the interface is firstly loaded.
     */
    virtual void OnLoad() {}

    /**
     * @brief Called when switch to this interface.
     */
    virtual void OnMounted() {}

    /**
     * @brief Update the interface in each frame.
     * @param delta elapsed time since last update
     * @return Whether the interface should update in this frame.
     */
    virtual bool OnUpdate(DeltaTime /*delta*/) { return true; }

    /**
     * @brief Called when switch from this interface.
     */
    virtual void OnUnmounted() {}

    /**
     * @brief Called when the interface is unloaded.
     * @note This is only called when the application is closed.
     */
    virtual void OnUnload() {}

    /**
     * @brief Event handler for the interface.
     * @param event The event to handle. Guaranteed to be not handled.
     */
    virtual void OnEvent(const Ref<Event>& /*event*/) {}

    /**
     * @brief Render the interface in each frame.
     */
    virtual void OnRender() {}

    void PushLayer(const Ref<Layer>& layer);
    void PopLayer(const Ref<Layer>& layer);

    /**
     * @brief Pop the top layer.
     */
    void PopLayer();

    void PushOverlay(const Ref<Layer>& overlay);
    void PopOverlay(const Ref<Layer>& overlay);

    /**
     * @brief Pop the top overlay.
     */
    void PopOverlay();

private:
    // These are wrapper functions for the application to call.
    // So that certain actions can be done before and after the custom handlers.

    void InvokeOnLoad();
    void InvokeOnMounted();
    void InvokeOnUpdate(DeltaTime delta);
    void InvokeOnEvent(const Ref<Event>& event);
    void InvokeOnRender();
    void InvokeOnUnmounted();
    void InvokeOnUnload();

    bool _OnResize(WindowResizeEvent& event);

private:
    LayerStack _layers;
    std::string _name;

    int _width;
    int _height;

    // For now, we hide this camera to user, so they'll be limited to this
    // fixed view. :)
    Ref<InterfaceCamera> _camera;

    /**
     * @brief Use counter to handle recursive load/unload.
     */
    int _loaded;
};

// clang-format on

/**
 * @brief Get the built-in splash interface.
 * @return the built-in splash interface
 */
extern Interface* GetSplashInterface();

/**
 * @brief This must be implemented by the client to get custom interfaces.
 * @note An interface with name "Main" (case-sensitive) will be set as the main interface.
 *      Or you can change it in ApplicationSpecification.
 * @note Each interface should have different name, and it is recommended
 *      to make interface singleton.
 * @param name name of the interface
 * @return interface with the given name, nullptr if not found
 */
extern Interface* GetInterface(const std::string& name);

DGEX_END

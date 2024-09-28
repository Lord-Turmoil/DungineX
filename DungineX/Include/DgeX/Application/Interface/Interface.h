#pragma once

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Interface/LayerStack.h"

DGEX_BEGIN

class Layer;
class InterfaceCamera;

// clang-format off
/**
 * @brief Interface is a collection of layers, so that we can manage interface
 *      changes and layer stack easily.
 * @note Generally, you should override the following functions:
 *          OnLoad       -- will only be called once when the interface is firstly loaded
 *          OnMounted    -- every time when switch to this interface
 *          OnUnmounted  -- every time when switch from this interface
 *          OnUnload     -- will only be called once when the application is closed
 * @note Don't override OnUpdate unless you know what you are doing.
 */
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
     */
    virtual void OnUpdate(DeltaTime delta);

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
     * @param event event
     */
    virtual void OnEvent(const Ref<Event>& event) {}

    /**
     * @brief Render the interface in each frame.
     */
    virtual void OnRender() {}

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);

    void PushOverlay(Layer* overlay);
    void PopOverlay(Layer* overlay);

    const std::string& GetName() const { return _name; }
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }

private:
    // Some wrapper functions to hide the implementation details.
    void _OnLoad();
    void _OnEvent(const Ref<Event>& event);
    void _OnRender();

    bool _OnResize(WindowResizeEvent& event);

private:
    LayerStack _layers;
    std::string _name;

    int _width;
    int _height;

    // For now, we hide this camera to user, so they'll be limited to this
    // fixed view. :)
    Ref<InterfaceCamera> _camera;
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

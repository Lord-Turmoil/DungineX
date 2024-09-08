#pragma once

#include "DgeX/Application/Interface/LayerStack.h"

DGEX_BEGIN

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
    void OnEvent(const Ref<Event>&event);

    /**
     * @brief Render the interface in each frame.
     */
    void OnRender();

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);

    void PushOverlay(Layer* overlay);
    void PopOverlay(Layer* overlay);

    const std::string& GetName() const { return _name; }

private:
    LayerStack _layers;
    std::string _name;
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

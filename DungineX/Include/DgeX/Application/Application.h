#pragma once

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/EventBuffer.h"
#include "DgeX/Application/Interface/InterfaceStack.h"
#include "DgeX/Application/Window.h"

DGEX_BEGIN

struct ApplicationCommandLineArgs
{
    int Count = 0;
    char** Args = nullptr;

    const char* operator[](int index) const
    {
        DGEX_ASSERT(index < Count);
        return Args[index];
    }
};

struct ApplicationSpecification
{
    std::string Name = "DgeX Application"; // name of the application
    int Width = 1280;                      // initial width of the window
    int Height = 720;                      // initial height of the window
    WindowFlags Flags = Default;           // flags for the window

    std::string WorkingDirectory;
    ApplicationCommandLineArgs CommandLineArgs;
};

// forward declaration
class Interface;

/**
 * @brief Application represents the UI of the application.
 * @paragraph Application holds all Interfaces, while Interface holds all Layers.
 *          Application manages the transition of interfaces, and dispatch events
 *          to the current interface.
 */
class Application final
{
public:
    Application(ApplicationSpecification specification);
    ~Application();

    static Application* GetInstance()
    {
        return _sInstance;
    }

    void Run();

    void OnEvent(const Ref<Event>& event);

    Window* GetWindow() const
    {
        return _window.get();
    }

    void Close();

    const ApplicationSpecification& GetSpecification() const
    {
        return _specification;
    }

    int GetWidth() const
    {
        return _window->GetWidth();
    }

    int GetHeight() const
    {
        return _window->GetHeight();
    }

    glm::vec2 GetSize() const
    {
        return { static_cast<float>(GetWidth()), static_cast<float>(GetHeight()) };
    }

    // Update every second.
    float GetUpdateFps() const
    {
        return _updateFps;
    }

    // Update every second.
    float GetFps() const
    {
        return _renderFps;
    }

    /**
     * @brief De-sync the update and render interval.
     *
     * @paragraph
     *
     * By default, the application will run at the refresh rate of the monitor.
     * However, if you want higher FPS on a lower refresh rate monitor, you can
     * manually set the refresh rate. In this case, the application will update
     * at the desired refresh rate, but still render at the monitor's refresh rate.
     *
     * @note
     *
     * This is not recommended to set refresh rate higher than hardware, as
     * performance is not guaranteed.
     *
     * @param refreshRate The desired update refresh rate. 0 to sync with monitor.
     * @param sync Whether to sync with monitor's refresh rate.
     *             If is true, when refreshRate is the same as monitor's one,
     *             will ignore the refreshRate and just sync with monitor.
     */
    void SetFixedRefreshRate(int refreshRate, bool sync = true);

private:
    bool _OnWindowClose(WindowCloseEvent& e);
    bool _OnWindowResize(WindowResizeEvent& e);

    bool _OnInterfaceTransit(InterfaceTransitEvent& e);
    bool _OnInterfaceChange(InterfaceChangeEvent& e);
    bool _OnInterfaceClose(InterfaceCloseEvent& e);

    /**
     * @brief The run thread to prevent freezing on window events.
     */
    void _Run();

    bool _Update(DeltaTime delta); // returns true if the application should close
    void _Render() const;

private:
    ApplicationSpecification _specification;
    Scope<Window> _window;

    bool _isRunning = false;
    bool _isMinimized = false;

    timestamp_t _lastFrameTime = 0.0f;

    EventBuffer _eventBuffer;

    InterfaceStack _interfaces;
    Interface* _currentInterface = nullptr;
    Interface* _nextInterface = nullptr;

    float _renderFps = 0.0f;
    float _updateFps = 0.0f;

    float _updateInterval;
    float _renderInterval;

    bool _sync;

private:
    static Application* _sInstance;

    friend int main(int argc, char** argv);
};

/**
 * @brief This is to be defined in client.
 *        In this function, the application must be created via new.
 * @param args commandline arguments
 * @return created application
 */
extern Application* CreateApplication(ApplicationCommandLineArgs args);

DGEX_END

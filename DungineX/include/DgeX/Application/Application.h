#pragma once

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/EventBuffer.h"
#include "DgeX/Application/Interface/InterfaceStack.h"
#include "DgeX/Application/Window.h"
#include "DgeX/Core/Assert.h"
#include "DgeX/Core/DeltaTime.h"

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
// clang-format off
class Application final
{
public:
    Application(ApplicationSpecification specification);
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application();

    static Application* GetInstance() { return _sInstance; }

    void Run();
    void OnEvent(const Ref<Event>& event);
    void Close();

    Window* GetWindow() const { return _window.get(); }

    const ApplicationSpecification& GetSpecification() const { return _specification; }

    int GetWidth() const { return _window->GetWidth(); }
    int GetHeight() const { return _window->GetHeight(); }

    float GetFps() const
    {
        DGEX_ASSERT(_averageFrameTime != 0.0f);
        return 1.0f / _averageFrameTime;
    }

    int GetRefreshRate() const { return _window->GetRefreshRate(); }

private:
    void _MainLoop();

    void _Update(DeltaTime delta);
    void _Render() const;

    void _Launch();
    void _Shutdown();

private:
    bool _OnWindowClose(WindowCloseEvent& event);
    bool _OnWindowResize(WindowResizeEvent& event);

    bool _OnInterfaceTransit(InterfaceTransitEvent& event);
    bool _OnInterfaceChange(InterfaceChangeEvent& event);
    bool _OnInterfaceClose(InterfaceCloseEvent& event);


private:
    ApplicationSpecification _specification;
    Scope<Window> _window;

    bool _isRunning = false;
    bool _isMinimized = false;

    timestamp_t _lastFrameTime = 0.0f;
    timestamp_t _averageFrameTime = 0.0f;

    EventBuffer _eventBuffer;

    InterfaceStack _interfaces;
    Interface* _currentInterface = nullptr;

private:
    static Application* _sInstance;

    friend int main(int argc, char** argv);
};

// clang-format on

/**
 * @brief This is to be defined in client.
 *        In this function, the application must be created via new.
 * @param args commandline arguments
 * @return created application
 */
extern Application* CreateApplication(ApplicationCommandLineArgs args);

DGEX_END

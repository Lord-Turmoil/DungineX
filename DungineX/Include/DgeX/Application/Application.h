#pragma once

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/EventBuffer.h"
#include "DgeX/Application/Interface/InterfaceStack.h"
#include "DgeX/Application/Window.h"
#include "DgeX/Common/Assert.h"
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

    float GetFps() const
    {
        DGEX_ASSERT(_averageFrameTime != 0.0f);
        return 1.0f / _averageFrameTime;
    }

    int GetRefreshRate() const
    {
        return _window->GetRefreshRate();
    }

private:
    bool _OnWindowClose(WindowCloseEvent& event);
    bool _OnWindowResize(WindowResizeEvent& e);

    bool _OnInterfaceTransit(InterfaceTransitEvent& e);
    bool _OnInterfaceChange(InterfaceChangeEvent& e);
    bool _OnInterfaceClose(InterfaceCloseEvent& event);

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
    timestamp_t _averageFrameTime = 0.0f;

    EventBuffer _eventBuffer;

    InterfaceStack _interfaces;
    Interface* _currentInterface = nullptr;
    Interface* _nextInterface = nullptr;

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

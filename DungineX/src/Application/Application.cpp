#include "DgeX/Application/Application.h"

#include "DgeX/Application/Event/EventEmitter.h"
#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Core/Math.h"
#include "DgeX/Renderer/RenderCommand.h"
#include "DgeX/Renderer/Renderer.h"
#include "DgeX/Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>

DGEX_BEGIN

Application* Application::_sInstance = nullptr;

Application::Application(ApplicationSpecification specification) : _specification(std::move(specification))
{
    DGEX_ASSERT(!_sInstance, DGEX_MSG_APPLICATION_ALREADY_CREATED);
    _sInstance = this;

    // Set working directory here
    if (!_specification.WorkingDirectory.empty())
    {
        std::filesystem::current_path(_specification.WorkingDirectory);
    }

    _window = Window::Create(
        WindowProps(_specification.Name, _specification.Width, _specification.Height, _specification.Flags));
    _window->SetEventCallback(DGEX_BIND_EVENT_FN(Application::OnEvent));
    EventEmitter::SetEventCallBack(DGEX_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();
    Renderer::OnWindowResize(_window->GetWidth(), _window->GetHeight());
}

Application::~Application()
{
    Renderer::Shutdown();
}

void Application::Run()
{
    if (_isRunning)
    {
        DGEX_ASSERT(false, DGEX_MSG_APPLICATION_RUNNING);
        return;
    }
    _isRunning = true;

    _Launch();

    _window->Detach();
    std::thread thread(&Application::_MainLoop, this);
    while (_isRunning)
    {
        glfwWaitEvents();
    }
    thread.join();
    _window->Attach();

    _Shutdown();
}

void Application::OnEvent(const Ref<Event>& event)
{
    DGEX_CORE_TRACE("{0}", event->ToString());
    _eventBuffer.PushEvent(event);
}

void Application::Close()
{
    _isRunning = false;
}

void Application::_MainLoop()
{
#ifdef DGEX_DEBUG
    static timestamp_t elapsedTime;
#endif

    // We need to limit the delta time to prevent a sudden jump in the game.
    timestamp_t maxDelta = 2.0f * 1.0f / static_cast<timestamp_t>(_window->GetRefreshRate());

    _window->Attach();
    while (_isRunning)
    {
        timestamp_t time = Time::GetTimestamp();
        DeltaTime delta = Math::Min(maxDelta, time - _lastFrameTime);
        _lastFrameTime = time;
        _averageFrameTime = (_averageFrameTime == 0.0f) ? delta : (_averageFrameTime * 0.99f + delta * 0.01f);

        _Update(delta);
        _Render();

#ifdef DGEX_DEBUG
        elapsedTime += delta;
        if (elapsedTime > 5.0f)
        {
            DGEX_CORE_DEBUG("FPS: {0}", GetFps());
            elapsedTime = 0.0f;
        }
#endif
    }
    _window->Detach();
}

inline void Application::_Update(DeltaTime delta)
{
    _eventBuffer.SwapBuffer();

    for (auto& event : _eventBuffer)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(DGEX_BIND_EVENT_FN(Application::_OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DGEX_BIND_EVENT_FN(Application::_OnWindowResize));
        dispatcher.Dispatch<InterfaceTransitEvent>(DGEX_BIND_EVENT_FN(Application::_OnInterfaceTransit));
        dispatcher.Dispatch<InterfaceChangeEvent>(DGEX_BIND_EVENT_FN(Application::_OnInterfaceChange));
        dispatcher.Dispatch<InterfaceCloseEvent>(DGEX_BIND_EVENT_FN(Application::_OnInterfaceClose));
    }

    for (auto& event : _eventBuffer)
    {
        if (!event->Handled)
        {
            _currentInterface->InvokeOnEvent(event);
        }
    }

    _currentInterface->InvokeOnUpdate(delta);
}

inline void Application::_Render() const
{
    RenderCommand::ClearDevice();
    _currentInterface->InvokeOnRender();
    _window->OnRender();
}

void Application::_Launch()
{
    _currentInterface = GetSplashInterface();
    if (_currentInterface == nullptr)
    {
        DGEX_ASSERT(false, DGEX_MSG_NO_SPLASH_INTERFACE);
        return;
    }
    _currentInterface->InvokeOnLoad();
    _currentInterface->InvokeOnMounted();
    _interfaces.Push(_currentInterface);
}

void Application::_Shutdown()
{
    Interface* interface = _interfaces.Current();
    while (interface)
    {
        _interfaces.Pop();
        interface->InvokeOnUnmounted();
        interface->InvokeOnUnload();

        interface = _interfaces.Current();
    }
}

bool Application::_OnWindowClose(WindowCloseEvent& /*event*/)
{
    Close();
    return true;
}

bool Application::_OnWindowResize(WindowResizeEvent& event)
{
    _isMinimized = event.GetWidth() == 0 || event.GetHeight() == 0;
    if (!_isMinimized)
    {
        Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    }
    return false;
}

bool Application::_OnInterfaceTransit(InterfaceTransitEvent& event)
{
    DGEX_LOG_INFO("Interface transition: {0} -> {1}", event.GetFromName(), event.GetToName());

    Interface* nextInterface = GetInterface(event.GetToName());
    if (nextInterface == nullptr)
    {
        DGEX_LOG_ERROR("Interface not found: {0}", event.GetToName());
        return false;
    }

    _currentInterface->InvokeOnUnmounted();

    nextInterface->InvokeOnLoad();
    nextInterface->InvokeOnMounted();
    _interfaces.Push(nextInterface);

    _currentInterface = nextInterface;
    nextInterface = nullptr;

    return true;
}

bool Application::_OnInterfaceChange(InterfaceChangeEvent& e)
{
    DGEX_LOG_INFO("Interface change: {0} -> {1}", e.GetFromName(), e.GetToName());

    Interface* nextInterface = GetInterface(e.GetToName());
    if (nextInterface == nullptr)
    {
        DGEX_LOG_ERROR("Interface not found: {0}", e.GetToName());
        return false;
    }

    _interfaces.Pop();
    _currentInterface->InvokeOnUnmounted();
    _currentInterface->InvokeOnUnload();

    nextInterface->InvokeOnLoad();
    nextInterface->InvokeOnMounted();
    _interfaces.Push(nextInterface);

    _currentInterface = nextInterface;
    nextInterface = nullptr;

    return true;
}

bool Application::_OnInterfaceClose(InterfaceCloseEvent& /*event*/)
{
    if (!_currentInterface)
    {
        DGEX_LOG_ERROR("No interface to close");
        return false;
    }

    _interfaces.Pop();
    _currentInterface->InvokeOnUnmounted();
    _currentInterface->InvokeOnUnload();

    DGEX_LOG_INFO("Interface {0} closed", _currentInterface->GetName());

    _currentInterface = _interfaces.Current();
    if (!_currentInterface)
    {
        // No interface left...
        Close();
    }
    else
    {
        _currentInterface->OnMounted();
    }

    return true;
}

DGEX_END

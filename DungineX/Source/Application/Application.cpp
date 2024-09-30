#include <cstdio>

#include "DgeX/Application/Application.h"
#include "DgeX/Application/Event/EventEmitter.h"
#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Renderer/RenderCommand.h"
#include "DgeX/Renderer/Renderer.h"
#include "DgeX/Utils/PlatformUtils.h"
#include "GLFW/glfw3.h"

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

    // Initialize splash interface
    _currentInterface = GetSplashInterface();
    if (_currentInterface == nullptr)
    {
        DGEX_ASSERT(false, DGEX_MSG_NO_SPLASH_INTERFACE);
        return;
    }
    _currentInterface->_OnLoad();
    _currentInterface->OnMounted();

    _window->Detach();

    std::thread thread(&Application::_Run, this);
    while (_isRunning)
    {
        glfwWaitEvents();
    }
    thread.join();

    _window->Attach();
}

void Application::OnEvent(const Ref<Event>& event)
{
    DGEX_CORE_TRACE("{0}", event->ToString());
    _eventBuffer.PushEvent(event);
}

void Application::Close()
{
    _isRunning = false;
    Interface* interface = _interfaces.CurrentInterface();
    while (interface)
    {
        interface->OnUnmounted();
        interface->OnUnload();

        _interfaces.PopInterface();
        interface = _interfaces.CurrentInterface();
    }
}

bool Application::_OnWindowClose(WindowCloseEvent& event)
{
    Close();
    return true;
}

bool Application::_OnWindowResize(WindowResizeEvent& e)
{
    _isMinimized = e.GetWidth() == 0 || e.GetHeight() == 0;
    if (!_isMinimized)
    {
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    }
    return false;
}

bool Application::_OnInterfaceTransit(InterfaceTransitEvent& e)
{
    DGEX_LOG_INFO("Interface transition: {0} -> {1}", e.GetFromName(), e.GetToName());

    _nextInterface = GetInterface(e.GetToName());
    if (_nextInterface == nullptr)
    {
        DGEX_LOG_ERROR("Interface not found: {0}", e.GetToName());
        return false;
    }

    _currentInterface->OnUnmounted();
    if (_interfaces.PushInterface(_nextInterface))
    {
        _nextInterface->_OnLoad();
    }
    _nextInterface->OnMounted();

    _currentInterface = _nextInterface;
    _nextInterface = nullptr;

    return true;
}

bool Application::_OnInterfaceChange(InterfaceChangeEvent& e)
{
    DGEX_LOG_INFO("Interface change: {0} -> {1}", e.GetFromName(), e.GetToName());

    _nextInterface = GetInterface(e.GetToName());
    if (_nextInterface == nullptr)
    {
        DGEX_LOG_ERROR("Interface not found: {0}", e.GetToName());
        return false;
    }

    Interface* interface = _interfaces.CurrentInterface();
    while (interface)
    {
        interface->OnUnmounted();
        interface->OnUnload();
        _interfaces.PopInterface();
        interface = _interfaces.CurrentInterface();
    }
    _interfaces.ClearInterfaces();

    if (_interfaces.PushInterface(_nextInterface))
    {
        _nextInterface->_OnLoad();
    }
    _nextInterface->OnMounted();

    _currentInterface = _nextInterface;
    _nextInterface = nullptr;

    return true;
}

bool Application::_OnInterfaceClose(InterfaceCloseEvent& event)
{
    if (!_currentInterface)
    {
        DGEX_LOG_ERROR("No interface to close");
        return false;
    }

    DGEX_LOG_INFO("Interface {0} closed", _currentInterface->GetName());

    _currentInterface->OnUnmounted();
    _interfaces.PopInterface();

    _currentInterface = _interfaces.CurrentInterface();
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

void Application::_Run()
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
        DeltaTime delta = time - _lastFrameTime;
        _lastFrameTime = time;

        if (delta > maxDelta)
        {
            // Skip the frame update if the delta time is too large.
            _Render();
            continue;
        }

        if (_Update(delta))
        {
            break;
        }
        _Render();

        if (_averageFrameTime == 0.0f)
        {
            _averageFrameTime = delta;
        }
        else
        {
            _averageFrameTime = _averageFrameTime * 0.99f + delta * 0.01f;
        }

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

inline bool Application::_Update(DeltaTime delta)
{
    _eventBuffer.SwapBuffer();
    // Dispatch window events
    for (auto& event : _eventBuffer)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(DGEX_BIND_EVENT_FN(Application::_OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DGEX_BIND_EVENT_FN(Application::_OnWindowResize));
        dispatcher.Dispatch<InterfaceTransitEvent>(DGEX_BIND_EVENT_FN(Application::_OnInterfaceTransit));
        dispatcher.Dispatch<InterfaceChangeEvent>(DGEX_BIND_EVENT_FN(Application::_OnInterfaceChange));
        dispatcher.Dispatch<InterfaceCloseEvent>(DGEX_BIND_EVENT_FN(Application::_OnInterfaceClose));
    }
    if (!_isRunning)
    {
        return true;
    }
    for (auto& event : _eventBuffer)
    {
        if (!event->Handled)
        {
            _currentInterface->_OnEvent(event);
        }
    }
    _currentInterface->OnUpdate(delta);

    return false;
}

inline void Application::_Render() const
{
    RenderCommand::ClearDevice();
    _currentInterface->_OnRender();
    _window->OnRender();
}

DGEX_END

#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Application/Application.h"
#include "DgeX/Application/Interface/Layer.h"
#include "DgeX/Core/UUID.h"
#include "DgeX/Renderer/Camera/InterfaceCamera.h"
#include "DgeX/Renderer/RenderApi.h"

DGEX_BEGIN

Interface::Interface(const std::string& name) : _width(0), _height(0)
{
    if (name.empty())
    {
        _name = "Interface ";
        _name += UUID().ToString();
    }
    else
    {
        _name = name;
    }
}

void Interface::OnUpdate(DeltaTime delta)
{
    for (auto layer : _layers)
    {
        layer->OnUpdate(delta);
    }
}

void Interface::PushLayer(Layer* layer)
{
    layer->_SetParent(this);
    _layers.PushLayer(layer);
    layer->OnAttach();
}

void Interface::PopLayer(Layer* layer)
{
    layer->OnDetach();
    _layers.PopLayer(layer);
    layer->_SetParent(nullptr);
}

void Interface::PushOverlay(Layer* overlay)
{
    overlay->_SetParent(this);
    _layers.PushOverlay(overlay);
    overlay->OnAttach();
}

void Interface::PopOverlay(Layer* overlay)
{
    overlay->OnDetach();
    _layers.PopOverlay(overlay);
    overlay->_SetParent(nullptr);
}

void Interface::_OnLoad()
{
    DGEX_CORE_INFO("Prepare to load interface: {0}", _name);
    _width = Application::GetInstance()->GetWidth();
    _height = Application::GetInstance()->GetHeight();
    if (!_camera)
    {
        _camera = CreateRef<InterfaceCamera>(static_cast<float>(_width), static_cast<float>(_height));
    }
    else
    {
        _camera->OnResize(static_cast<float>(_width), static_cast<float>(_height));
    }

    DGEX_TIME_BEGIN("Load interface");
    OnLoad();
    DGEX_TIME_END();

    DGEX_LOG_INFO("Loaded interface: {0}", _name);
}

void Interface::_OnMounted()
{
    DGEX_TIME_BEGIN("Mount interface");
    OnMounted();
    DGEX_TIME_END();

    DGEX_LOG_INFO("Mounted interface: {0}", _name);
}

void Interface::_OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(DGEX_BIND_EVENT_FN(Interface::_OnResize));

    for (auto it = _layers.rbegin(); it != _layers.rend(); ++it)
    {
        if (event->Handled)
        {
            break;
        }
        (*it)->OnEvent(event);
    }

    OnEvent(event);
}

void Interface::_OnRender()
{
    RenderApi::BeginScene(*_camera);
    for (auto layer : _layers)
    {
        layer->OnRender();
    }
    RenderApi::EndScene();

    OnRender();
}

void Interface::_OnUnmounted()
{
    DGEX_TIME_BEGIN("Unmount interface");
    OnUnmounted();
    DGEX_TIME_END();

    DGEX_LOG_INFO("Unmounted interface: {0}", _name);
}

void Interface::_OnUnload()
{
    DGEX_TIME_BEGIN("Unload interface");
    OnUnload();
    DGEX_TIME_END();

    DGEX_LOG_INFO("Unloaded interface: {0}", _name);
}

bool Interface::_OnResize(WindowResizeEvent& /*event*/)
{
    _width = Application::GetInstance()->GetWidth();
    _height = Application::GetInstance()->GetHeight();
    _camera->OnResize(static_cast<float>(_width), static_cast<float>(_height));

    return false;
}

DGEX_END

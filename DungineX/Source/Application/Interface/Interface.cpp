#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Core/UUID.h"
#include "DgeX/Renderer/RenderApi.h"

DGEX_BEGIN

Interface::Interface(const std::string& name)
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

void Interface::OnEvent(const Ref<Event>& event)
{
    for (auto it = _layers.rbegin(); it != _layers.rend(); ++it)
    {
        if (event->Handled)
            break;
        (*it)->OnEvent(event);
    }
}

void Interface::OnRender()
{
    RenderApi::ClearDevice();
    for (auto layer : _layers)
    {
        layer->OnRender();
    }
}

void Interface::PushLayer(Layer* layer)
{
    layer->SetParent(this);
    _layers.PushLayer(layer);
    layer->OnAttach();
}

void Interface::PopLayer(Layer* layer)
{
    layer->OnDetach();
    _layers.PopLayer(layer);
    layer->SetParent(nullptr);
}

void Interface::PushOverlay(Layer* overlay)
{
    overlay->SetParent(this);
    _layers.PushOverlay(overlay);
    overlay->OnAttach();
}

void Interface::PopOverlay(Layer* overlay)
{
    overlay->OnDetach();
    _layers.PopOverlay(overlay);
    overlay->SetParent(nullptr);
}

DGEX_END

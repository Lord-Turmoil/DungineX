#include "DgeX/Application/Interface/LayerStack.h"

DGEX_BEGIN

void LayerStack::PushLayer(Layer* layer)
{
    _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
    _layerInsertIndex++;
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
    if (it != _layers.begin() + _layerInsertIndex)
    {
        _layers.erase(it);
        _layerInsertIndex--;
    }
}

void LayerStack::PushOverlay(Layer* overlay)
{
    _layers.emplace_back(overlay);
}

void LayerStack::PopOverlay(Layer* overlay)
{
    auto it = std::find(_layers.begin() + _layerInsertIndex, _layers.end(), overlay);
    if (it != _layers.end())
    {
        _layers.erase(it);
    }
}

DGEX_END

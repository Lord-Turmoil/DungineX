#include "DgeX/Application/Interface/LayerStack.h"

#include "DgeX/Application/Interface/Layer.h"

DGEX_BEGIN

void LayerStack::PushLayer(const Ref<Layer>& layer)
{
    _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
    _layerInsertIndex++;
}

void LayerStack::PopLayer(const Ref<Layer>& layer)
{
    auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
    if (it != _layers.begin() + _layerInsertIndex)
    {
        _layers.erase(it);
        _layerInsertIndex--;
    }
}

Ref<Layer> LayerStack::GetTopLayer() const
{
    if (_layerInsertIndex > 0)
    {
        return _layers[_layerInsertIndex - 1];
    }
    return nullptr;
}

void LayerStack::PushOverlay(const Ref<Layer>& overlay)
{
    _layers.emplace_back(overlay);
}

void LayerStack::PopOverlay(const Ref<Layer>& overlay)
{
    auto it = std::find(_layers.begin() + _layerInsertIndex, _layers.end(), overlay);
    if (it != _layers.end())
    {
        _layers.erase(it);
    }
}

Ref<Layer> LayerStack::GetTopOverlay() const
{
    if (_layerInsertIndex < _layers.size())
    {
        return _layers.back();
    }
    return nullptr;
}

DGEX_END

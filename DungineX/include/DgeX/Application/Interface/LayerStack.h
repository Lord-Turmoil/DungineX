#pragma once

#include "DgeX/Core/Base.h"

#include <vector>

DGEX_BEGIN

class Layer;

/**
 * @brief LayerStack makes it easier to manage layers.
 */
// clang-format off
class LayerStack
{
public:
	LayerStack() = default;
    LayerStack(const LayerStack& other) = delete;
    LayerStack(LayerStack&& other) noexcept = delete;
    LayerStack& operator=(const LayerStack& other) = delete;
    LayerStack& operator=(LayerStack&& other) noexcept = delete;

    ~LayerStack() = default;

	void PushLayer(const Ref<Layer>& layer);
	void PopLayer(const Ref<Layer>& layer);
    Ref<Layer> GetTopLayer() const;

    void PushOverlay(const Ref<Layer>& overlay);
	void PopOverlay(const Ref<Layer>& overlay);
    Ref<Layer> GetTopOverlay() const;

	std::vector<Ref<Layer>>::iterator begin() { return _layers.begin(); }
	std::vector<Ref<Layer>>::iterator end() { return _layers.end(); }
	std::vector<Ref<Layer>>::reverse_iterator rbegin() { return _layers.rbegin(); }
	std::vector<Ref<Layer>>::reverse_iterator rend() { return _layers.rend(); }

	std::vector<Ref<Layer>>::const_iterator begin() const { return _layers.begin(); }
	std::vector<Ref<Layer>>::const_iterator end()	const { return _layers.end(); }
	std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
	std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return _layers.rend(); }

private:
	/**
	 * @note Layers are not managed by the LayerStack, so we don't need to delete them.
	 */
	std::vector<Ref<Layer>> _layers;
	int _layerInsertIndex = 0;
};

// clang-format on

DGEX_END

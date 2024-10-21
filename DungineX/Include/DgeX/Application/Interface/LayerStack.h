#pragma once

DGEX_BEGIN

class Layer;

// clang-format off
/**
 * @brief LayerStack makes it easier to manage layers.
 */
class LayerStack
{
public:
	LayerStack() = default;
    LayerStack(const LayerStack& other) = delete;
    LayerStack(LayerStack&& other) noexcept = delete;
    LayerStack& operator=(const LayerStack& other) = delete;
    LayerStack& operator=(LayerStack&& other) noexcept = delete;
    ~LayerStack() = default;

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

    void PushOverlay(Layer* overlay);
	void PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator begin() { return _layers.begin(); }
	std::vector<Layer*>::iterator end() { return _layers.end(); }
	std::vector<Layer*>::reverse_iterator rbegin() { return _layers.rbegin(); }
	std::vector<Layer*>::reverse_iterator rend() { return _layers.rend(); }

	std::vector<Layer*>::const_iterator begin() const { return _layers.begin(); }
	std::vector<Layer*>::const_iterator end()	const { return _layers.end(); }
	std::vector<Layer*>::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
	std::vector<Layer*>::const_reverse_iterator rend() const { return _layers.rend(); }

private:
	/**
	 * @note Layers are not managed by the LayerStack, so we don't need to delete them.
	 */
	std::vector<Layer*> _layers;
	int _layerInsertIndex = 0;
};

// clang-format on

DGEX_END

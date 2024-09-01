#include <ostream>
#include <string>

#include "core/app/layer_stack.h"

namespace CG {

    LayerStack::LayerStack() {}

    LayerStack::~LayerStack() {
        for (Layer* layer : layers_) {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer) {
        layers_.emplace(layers_.begin() + layerInsertIndex_, layer);
        layerInsertIndex_++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* overlay) {
        layers_.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto it = std::find(layers_.begin(), layers_.begin() + layerInsertIndex_, layer);
        if (it != layers_.begin() + layerInsertIndex_) {
            layer->OnDetach();
            layers_.erase(it);
            layerInsertIndex_--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay) {
        auto it = std::find(layers_.begin() + layerInsertIndex_, layers_.end(), overlay);
        if (it != layers_.end()) {
            overlay->OnDetach();
            layers_.erase(it);
        }
    }
} // namespace CG

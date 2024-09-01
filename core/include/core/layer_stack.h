#pragma once

#include <string>
#include <vector>

#include "core/layer.h"

namespace CG {

    class LayerStack {
        // TODO: 添加更多迭代器的特性
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return layers_.begin(); }
        std::vector<Layer*>::iterator end() { return layers_.end(); }

        std::vector<Layer*>::const_iterator begin() const { return layers_.cbegin(); }
        std::vector<Layer*>::const_iterator end() const { return layers_.cend(); }

    private:
        std::vector<Layer*> layers_;
        uint32_t layerInsertIndex_ {};
    };

} // namespace CG

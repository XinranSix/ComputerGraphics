#pragma once

#include <string>

#include "core/events/event.h"
#include "core/timestep.h"

namespace CG {

    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return debugName_; }

    protected:
        std::string debugName_;
    };

} // namespace CG

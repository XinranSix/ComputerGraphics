#pragma once

#include <string>

#include "core/timestep.h"
#include "events/event.h"

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

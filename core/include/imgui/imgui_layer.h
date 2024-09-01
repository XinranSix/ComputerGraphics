#pragma once

#include "core/app/layer.h"
#include "events/mouse_event.h"

namespace CG {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void Begin();
        void End();

        virtual void OnEvent(Event& event) override;
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void SetDarkThemeColors();

    private:
        float time_ = {};
    };

} // namespace CG

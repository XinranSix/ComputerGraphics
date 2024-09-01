#pragma once

#include <string>

#include "core/events/application_event.h"
#include "core/events/event.h"
#include "core/layer_stack.h"
#include "core/window.h"
#include "imgui/imgui_layer.h"

namespace CG {

    class Application {
    public:
        Application(const std::string& name = "OpenGL Sandbox", uint32_t width = 1280, uint32_t height = 720);
        virtual ~Application() = default;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() { return *window_; }

        inline static Application& Get() { return *sInstance_; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        std::unique_ptr<Window> window_;
        ImGuiLayer* imguiLayer_;
        bool running_ = true;
        LayerStack layerStack_;
        float lastFrameTime_ {};

    private:
        static Application* sInstance_;
    };

} // namespace CG

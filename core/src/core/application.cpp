// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "core/application.h"
#include "core/input.h"
#include "core/log.h"

namespace CG {
    // TODO: use lambda instead of std::bind
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::sInstance_ = nullptr;

    Application::Application(const std::string& name, uint32_t width, uint32_t height) {
        if (!sInstance_) {
            // Initialize core
            Logger::Init();
        }

        CG_CORE_ASSERT(!sInstance_, "Application already exists!");
        sInstance_ = this;

        window_ = std::unique_ptr<Window>(Window::Create({ name, width, height }));
        window_->SetEventCallback(BIND_EVENT_FN(OnEvent));

        // Renderer::Init();

        imguiLayer_ = new ImGuiLayer();
        PushOverlay(imguiLayer_);
    }

    void Application::PushLayer(Layer* layer) { layerStack_.PushLayer(layer); }

    void Application::PushOverlay(Layer* layer) { layerStack_.PushOverlay(layer); }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = layerStack_.end(); it != layerStack_.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled) {
                break;
            }
        }
    }

    void Application::Run() {
        while (running_) {
            float time = (float)glfwGetTime();
            Timestep timestep = time - lastFrameTime_;
            lastFrameTime_ = time;

            for (Layer* layer : layerStack_) {
                layer->OnUpdate(timestep);
            }

            imguiLayer_->Begin();
            for (Layer* layer : layerStack_) {
                layer->OnImGuiRender();
            }
            imguiLayer_->End();

            window_->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        running_ = false;
        return true;
    }

} // namespace CG

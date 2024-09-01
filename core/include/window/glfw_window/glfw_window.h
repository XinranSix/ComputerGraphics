#pragma once

#include <string>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "core/window.h"

namespace CG {

    class GLFWWindow : public Window {
    public:
        GLFWWindow(const WindowProps& props);
        virtual ~GLFWWindow();

        void OnUpdate() override;

        inline uint32_t GetWidth() const override { return data_.width; }
        inline uint32_t GetHeight() const override { return data_.height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { data_.eventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const override { return window_; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* window_;

        struct WindowData {
            std::string title;
            uint32_t width, height;
            bool vSync;

            EventCallbackFn eventCallback;
        };

        WindowData data_;
    };
} // namespace CG

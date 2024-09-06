// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "window/glfw/glfw_window.h"
#include "core/log/log.h"
#include "events/application_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"

namespace CG {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        CG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props) { return new GLFWWindow(props); }

    GLFWWindow::GLFWWindow(const WindowProps& props) { Init(props); }

    GLFWWindow::~GLFWWindow() { Shutdown(); }

    void GLFWWindow::Init(const WindowProps& props) {
        data_.title = props.title;
        data_.width = props.width;
        data_.height = props.height;

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            CG_CORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;

           glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
              glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
              glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
        }

        window_ = glfwCreateWindow((int)props.width, (int)props.height, data_.title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(window_);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CG_CORE_ASSERT(status, "Failed to initialize Glad!");

        CG_CORE_INFO("OpenGL Info:");
        CG_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        CG_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        CG_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

        glfwSetWindowUserPointer(window_, &data_);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(window_, [](GLFWwindow* window, uint32_t keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.eventCallback(event);
        });
    }

    void GLFWWindow::Shutdown() { glfwDestroyWindow(window_); }

    void GLFWWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(window_);
    }

    void GLFWWindow::SetVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        data_.vSync = enabled;
    }

    bool GLFWWindow::IsVSync() const { return data_.vSync; }

} // namespace CG

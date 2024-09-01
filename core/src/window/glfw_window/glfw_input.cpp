// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "window/glfw_window/glfw_input.h"
#include "core/app/application.h"

namespace CG {

    Input* Input::sInstance = new GLFWInput();

    bool GLFWInput::IsKeyPressedImpl(int keycode) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GLFWInput::IsMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> GLFWInput::GetMousePositionImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float GLFWInput::GetMouseXImpl() {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float GLFWInput::GetMouseYImpl() {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }

} // namespace CG

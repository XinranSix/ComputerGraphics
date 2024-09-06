#include "sandbox_layer.h"
#include "GLFW/glfw3.h"
#include "core/app/application.h"
#include "imgui.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

SandboxLayer::SandboxLayer() {
    glfwSetFramebufferSizeCallback((GLFWwindow*)CG::Application::Get().GetWindow().GetNativeWindow(),
                                   framebuffer_size_callback);
}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {

    // CG::GL::Utils::EnableGLDebugging();
    // Init here
    // framebuffer_.
    // CG::GL::FramebufferSpecification spec;
}

void SandboxLayer::OnDetach() {
    // Shutdown here
}

void SandboxLayer::OnEvent(CG::Event& event) {
    // Events here
}

void SandboxLayer::OnUpdate(CG::Timestep ts) {
    // Render here
    glClearColor(background_color_[0], background_color_[1], background_color_[2], background_color_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glfwSwapBuffers((GLFWwindow*)CG::Application::Get().GetWindow().GetNativeWindow());

    // glfwPollEvents();
}

void SandboxLayer::OnImGuiRender() {
    // ImGui here
    ImGui::ColorEdit4("背景颜色", background_color_);
    ImGui::ColorEdit4("Background Color", background_color_);
}

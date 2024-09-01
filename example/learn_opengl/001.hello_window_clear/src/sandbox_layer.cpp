#include <format>

#include "imgui.h"
#include "opengl/opengl_debug.h"
#include "sandbox_layer.h"

SandboxLayer::SandboxLayer() {}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {

    CG::Utils::EnableGLDebugging();
    // Init here
    // framebuffer_.
    CG::GL::FramebufferSpecification spec;
}

void SandboxLayer::OnDetach() {
    // Shutdown here
}

void SandboxLayer::OnEvent(CG::Event& event) {
    // Events here
}

void SandboxLayer::OnUpdate(CG::Timestep ts) {
    // Render here
    glClearColor(backgroundcolor_[0], backgroundcolor_[1], backgroundcolor_[2], backgroundcolor_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SandboxLayer::OnImGuiRender() {
    // ImGui here
    ImGui::ColorEdit4("背景颜色", backgroundcolor_);
    ImGui::ColorEdit4("Background Color", backgroundcolor_);

    ImGui::Begin("OpenGL Info:");
    ImGui::Text("Vendor: %s", (const char*)glGetString(GL_VENDOR));
    ImGui::Text("Renderer: %s", (const char*)glGetString(GL_RENDERER));
    ImGui::Text("Version: %s", (const char*)glGetString(GL_VERSION));
    ImGui::End();
}

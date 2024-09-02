#include "sandbox_layer.h"
#include "GLFW/glfw3.h"
#include "graphics_api/opengl/renderer/opengl_debug.h"
#include "imgui.h"

SandboxLayer::SandboxLayer() {}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {

    CG::GL::Utils::EnableGLDebugging();

    // Init here
}

void SandboxLayer::OnDetach() {
    // Shutdown here
}

void SandboxLayer::OnEvent(CG::Event& event) {
    // Events here
}

void SandboxLayer::OnUpdate(CG::Timestep ts) {
    // Render here
}

void SandboxLayer::OnImGuiRender() {
    // ImGui here
}

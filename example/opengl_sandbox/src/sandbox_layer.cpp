#include "sandbox_layer.h"
#include "opengl/opengl_debug.h"

SandboxLayer::SandboxLayer() {}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {

    CG::Utils::EnableGLDebugging();

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

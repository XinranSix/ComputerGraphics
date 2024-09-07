#include "sandbox_layer.h"
#include "GLFW/glfw3.h"
#include "core/app/application.h"
#include "imgui.h"
#include "renderer/shader.h"
#include "renderer/vertex_array.h"
#include "renderer/vertex_buffer.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

SandboxLayer::SandboxLayer() {}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {
    glfwSetFramebufferSizeCallback((GLFWwindow*)CG::Application::Get().GetWindow().GetNativeWindow(),
                                   framebuffer_size_callback);

    shader_ = CG::Shader::Create("triangle", vertexShaderSource_, fragmentShaderSource_);

    auto vertexBuffer = CG::VertexBuffer::Create((float*)vertices_, sizeof(vertices_));
    vertexBuffer->SetLayout({
        { CG::ShaderDataType::Float3, "aPos" },
    });

    vertexArray_ = CG::VertexArray::Create();
    vertexArray_->AddVertexBuffer(vertexBuffer);
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

    glPolygonMode(GL_FRONT_AND_BACK, wireMode_ ? GL_LINE : GL_FILL);
    // draw our first triangle
    shader_->Bind();
    vertexArray_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SandboxLayer::OnImGuiRender() {
    // ImGui here
    ImGui::ColorEdit4("背景颜色", background_color_);
    ImGui::Checkbox("线框模式", &wireMode_);
}

#include <imgui.h>

#include "example_layer.h"
#include "glm/gtc/type_ptr.hpp"
#include "opengl/opengl_debug.h"

ExampleLayer::ExampleLayer() : cameraController_ { 16.0f / 9.0f } {}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::OnAttach() {
    CG::Utils::EnableGLDebugging();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_ = CG::Utils::Shader::FromGLSLTextFiles("assets/shader/test.frag.glsl", "assets/shader/test.frag.glsl");

    glCreateVertexArrays(1, &quadVA_);
    glBindVertexArray(quadVA_);

    float vertices[] { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f };

    glCreateBuffers(1, &quadVB_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVB_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
    glCreateBuffers(1, &quadIB_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void ExampleLayer::OnDetach() {
    glDeleteVertexArrays(1, &quadVA_);
    glDeleteBuffers(1, &quadVB_);
    glDeleteBuffers(1, &quadIB_);
}

void ExampleLayer::OnEvent(CG::Event& event) {
    cameraController_.OnEvent(event);

    CG::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<CG::MouseButtonPressedEvent>([&](CG::MouseButtonPressedEvent& e) {
        squareColor_ = squareAlternateColor_;
        return false;
    });
    dispatcher.Dispatch<CG::MouseButtonReleasedEvent>([&](CG::MouseButtonReleasedEvent& e) {
        squareColor_ = squareBaseColor_;
        return false;
    });
}

void ExampleLayer::OnUpdate(CG::Timestep ts) {
    cameraController_.OnUpdate(ts);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_->GetRendererID());

    int location = glGetUniformLocation(shader_->GetRendererID(), "u_ViewProjection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraController_.GetCamera().GetViewProjectionMatrix()));

    location = glGetUniformLocation(shader_->GetRendererID(), "u_Color");
    glUniform4fv(location, 1, glm::value_ptr(squareColor_));

    glBindVertexArray(quadVA_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ExampleLayer::OnImGuiRender() {
    ImGui::Begin("Controls");
    if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(squareBaseColor_))) {
        squareColor_ = squareBaseColor_;
    }
    ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(squareAlternateColor_));
    ImGui::End();
}

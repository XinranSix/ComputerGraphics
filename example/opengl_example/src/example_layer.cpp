#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "example_layer.h"
#include "graphics_api/opengl/renderer/opengl_debug.h"

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

    glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_->GetRendererID());

    int location = glGetUniformLocation(shader_->GetRendererID(), "u_ViewProjection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraController_.GetCamera().GetViewProjectionMatrix()));

    location = glGetUniformLocation(shader_->GetRendererID(), "u_Color");
    glUniform4fv(location, 1, glm::value_ptr(squareColor_));

    glBindVertexArray(quadVA_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f,
                            float columnWidth = 100.0f) {
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 2 * boldFont->FontSize);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

void ExampleLayer::OnImGuiRender() {
    ImGui::Begin("Controls");
    if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(squareBaseColor_))) {
        squareColor_ = squareBaseColor_;
    }
    ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(squareAlternateColor_));
    ImGui::End();

    ImGui::ColorEdit4("背景色", backgroundColor_);

    DrawVec3Control("平移", transform_.translation);
    glm::vec3 rotation = glm::degrees(transform_.rotation);
    DrawVec3Control("旋转", rotation);
    transform_.rotation = glm::radians(rotation);
    DrawVec3Control("伸缩", transform_.scale, 1.0f);
}

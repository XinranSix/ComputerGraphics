#pragma once

#include "core/app/layer.h"
#include "esc/components.h"
#include "graphics_api/opengl/renderer/orthographic_camera_controller.h"
#include "graphics_api/opengl/renderer/shader.h"

class ExampleLayer : public CG::Layer {
public:
    ExampleLayer();
    virtual ~ExampleLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(CG::Event& event) override;
    virtual void OnUpdate(CG::Timestep ts) override;
    virtual void OnImGuiRender() override;

private:
    CG::GL ::Shader* shader_;
    CG::Utils::OrthographicCameraController cameraController_;

    GLuint quadVA_, quadVB_, quadIB_;

    glm::vec4 squareBaseColor_ { 0.8f, 0.2f, 0.3f, 1.0f };
    glm::vec4 squareAlternateColor_ { 0.2f, 0.3f, 0.8f, 1.0f };
    glm::vec4 squareColor_ = squareBaseColor_;
    float backgroundColor_[4] { 0.1f, 0.1f, 0.1f, 1.0f };

    CG::TransformComponent transform_;
};

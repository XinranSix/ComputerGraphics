#pragma once

#include "core/layer.h"
#include "opengl/orthographic_camera_controller.h"
#include "opengl/shader.h"

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
    CG::Utils ::Shader* shader_;
    CG::Utils::OrthographicCameraController cameraController_;

    GLuint quadVA_, quadVB_, quadIB_;

    glm::vec4 squareBaseColor_ { 0.8f, 0.2f, 0.3f, 1.0f };
    glm::vec4 squareAlternateColor_ { 0.2f, 0.3f, 0.8f, 1.0f };
    glm::vec4 squareColor_ = squareBaseColor_;
};

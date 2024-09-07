#pragma once

#include <memory>

#include "core/app/layer.h"
#include "core/base/base.h"
#include "core/timestep.h"
#include "events/event.h"
#include "graphics_api/opengl/renderer/framebuffer.h"
#include "renderer/shader.h"

class SandboxLayer : public CG::Layer {
public:
    SandboxLayer();
    virtual ~SandboxLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(CG::Event& event) override;
    virtual void OnUpdate(CG::Timestep ts) override;
    virtual void OnImGuiRender() override;

private:
    float background_color_[4] { 0.7137f, 0.7333f, 0.7686f, 1.0f };
    std::shared_ptr<CG::GL::Framebuffer> framebuffer_;

    const char* vertexShaderSource_ {
        R"(
        #version 450 core

        layout (location = 0) in vec3 aPos;

        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )"
    };

    const char* fragmentShaderSource_ {
        R"(
        #version 450 core

        out vec4 FragColor;

        void main() {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )"
    };

    CG::Ref<CG::Shader> shader_;

    const GLfloat vertices_[9] {
        -0.5f, -0.5f, 0.0f, // left
        0.5f,  -0.5f, 0.0f, // right
        0.0f,  0.5f,  0.0f  // top
    };

    GLuint VBO_ {}, VAO_ {};

    bool wireMode_ {};
};

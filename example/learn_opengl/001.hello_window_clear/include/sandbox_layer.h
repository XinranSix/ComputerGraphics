#pragma once

#include <memory>

#include "core/app/layer.h"
#include "core/timestep.h"
#include "events/event.h"
#include "graphics_api/opengl/renderer/framebuffer.h"

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
};

#pragma once

#include <memory>

#include "core/events/event.h"
#include "core/layer.h"
#include "core/timestep.h"
#include "opengl/framebuffer.h"

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
    float backgroundcolor_[4] { 0.7137f, 0.7333f, 0.7686f, 1.0f };
    std::shared_ptr<CG::GL::Framebuffer> framebuffer_;
};

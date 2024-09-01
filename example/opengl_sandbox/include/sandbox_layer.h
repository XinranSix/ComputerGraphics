#pragma once

#include "core/events/event.h"
#include "core/layer.h"
#include "core/timestep.h"

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
};

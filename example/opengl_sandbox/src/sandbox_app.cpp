#include "core/app/application.h"
#include "sandbox_layer.h"

class Sandbox : public CG::Application {
public:
    Sandbox() { PushLayer(new SandboxLayer()); }
};

int main() {

    std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
    app->Run();

    return 0;
}

#include "core/application.h"
#include "sandbox_layer.h"

class SandboxApplication : public CG::Application {
public:
    SandboxApplication() { PushLayer(new SandboxLayer()); }
};

int main() {

    std::unique_ptr<SandboxApplication> app = std::make_unique<SandboxApplication>();
    app->Run();

    return 0;
}

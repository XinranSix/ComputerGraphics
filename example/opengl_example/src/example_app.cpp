#include "core/application.h"
#include "example_layer.h"

class Example : public CG::Application {
public:
    Example() : Application { "OpenGL Examples" } { PushLayer(new ExampleLayer()); }
};

int main() {

    std::unique_ptr<Example> app = std::make_unique<Example>();
    app->Run();

    return 0;
}

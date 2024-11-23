#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "load.h"
#include "shader.h"

class Application {
public:
    static Application& instance();

    void init(GLFWwindow* window);
    void display(GLFWwindow* window);

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

private:
    void placeDarkChess(size_t index, int x, int y);
    void placeLightChess(size_t index, int x, int y);
    float getOffset(int x, float step);

    Load* load_ {};
    Camera* camera_ {};
    Shader* shader_ {};

    std::vector<Object> objects_;

    int width_ = 1200;
    int height_ = 800;

    bool onOff_ = true;
};

#endif

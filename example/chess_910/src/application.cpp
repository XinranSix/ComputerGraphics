#include "application.h"

#include <iostream>

using namespace std;

namespace
{
constexpr float const step = 180.0F;
} // namespace

static constexpr char const *vs_source = R"(
    #version 330 core
    layout (location = 0) in vec4 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoords;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoords;

    void main() {
        FragPos = vec3(model * aPos);
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoords = aTexCoords;

        gl_Position = projection * view * model * aPos;
    }
)";

static constexpr char const *fs_source = R"(
    #version 330 core

    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoords;

    uniform vec3 viewPos;
    uniform vec3 lightPos;
    uniform bool onOff;

    uniform sampler2D textureDiffuse;

    out vec4 FragColor;

    void main() {
        vec3 color = texture(textureDiffuse, TexCoords).rgb;

        // ambient
        vec3 ambient = 0.3 * color;

        // diffuse
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 normal = normalize(Normal);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = color * spec;

        if(onOff){
            FragColor = vec4(ambient + diffuse + specular, 1.0);
        }
        else{
            FragColor = vec4(ambient, 1.0);
        }
    }
)";

Application &Application::instance()
{
    static Application instance;
    return instance;
}

void Application::init(GLFWwindow * /*window*/)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepthf(1.0);

    camera_ = new Camera();
    shader_ = new Shader(vs_source, fs_source);
    load_ = new Load();

    objects_ = load_->loadObject("object/Stone_Chess_Board/12951_Stone_Chess_Board_v1_L3.obj");
    if (!objects_.empty()) {
        std::vector<Object> tmp = load_->loadObject("object/Chess/chess.obj");
        objects_.insert(objects_.end(), tmp.begin(), tmp.end());
    }
    
    glUseProgram(shader_->id());
    glUniform1i(glGetUniformLocation(shader_->id(), "textureDiffuse"), 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void Application::display(GLFWwindow * /*window*/)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (objects_.empty())
    {
        return;
    }

    camera_->updateCurrentTime(glfwGetTime());
    glm::mat4 view = camera_->getViewMatrix();
    glm::mat4 projMatrix = glm::perspective(glm::radians(camera_->getFovy()),
                                            static_cast<float>(width()) / static_cast<float>(height()), 0.1F, 1000.0F);

    glUniformMatrix4fv(glGetUniformLocation(shader_->id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader_->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));

    glUniform3fv(glGetUniformLocation(shader_->id(), "viewPos"), 1, glm::value_ptr(camera_->getPosition()));
    glUniform3f(glGetUniformLocation(shader_->id(), "lightPos"), 10, 10, 0);
    glUniform1i(glGetUniformLocation(shader_->id(), "onOff"), static_cast<GLint>(onOff_));

    glm::mat4 model = glm::scale(glm::mat4{1.0}, glm::vec3(0.1));
    model = glm::rotate(model, glm::radians(90.0F), glm::vec3(-1, 0, 0));
    model = glm::translate(model, glm::vec3(0, -25.5, -0.5));
    glUniformMatrix4fv(glGetUniformLocation(shader_->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    objects_[0].draw();

    placeDarkChess(1, 0, 1);
    placeDarkChess(1, 1, 1);
    placeDarkChess(1, 2, 1);
    placeDarkChess(1, 3, 1);
    placeDarkChess(1, 4, 1);
    placeDarkChess(1, 5, 1);
    placeDarkChess(1, 6, 1);
    placeDarkChess(1, 7, 1);
    placeDarkChess(2, 0, 0);
    placeDarkChess(2, 7, 0);
    placeDarkChess(3, 1, 0);
    placeDarkChess(3, 6, 0);
    placeDarkChess(4, 2, 0);
    placeDarkChess(4, 5, 0);
    placeDarkChess(5, 3, 0);
    placeDarkChess(6, 4, 0);

    placeLightChess(7, 0, 6);
    placeLightChess(7, 1, 6);
    placeLightChess(7, 2, 6);
    placeLightChess(7, 3, 6);
    placeLightChess(7, 4, 6);
    placeLightChess(7, 5, 6);
    placeLightChess(7, 6, 6);
    placeLightChess(7, 7, 6);
    placeLightChess(8, 0, 7);
    placeLightChess(8, 7, 7);
    placeLightChess(9, 1, 7);
    placeLightChess(9, 6, 7);
    placeLightChess(10, 2, 7);
    placeLightChess(10, 5, 7);
    placeLightChess(11, 3, 7);
    placeLightChess(12, 4, 7);
}

float Application::getOffset(int x, float step)
{
    return static_cast<float>(3 - x + 0.5) * step;
}

void Application::placeDarkChess(size_t index, int x, int y)
{
    glm::mat4 model = scale(glm::mat4(1.0), glm::vec3(0.003));
    float offsetX = getOffset(x, -step);
    float offsetY = getOffset(y, -step);
    model = translate(model, glm::vec3(offsetX, 0, offsetY));

    glUniformMatrix4fv(glGetUniformLocation(shader_->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    objects_[index].draw();
}

void Application::placeLightChess(size_t index, int x, int y)
{
    glm::mat4 model = scale(glm::mat4(1.0), glm::vec3(0.003));
    model = rotate(model, glm::radians(180.0F), glm::vec3(0, 1, 0));
    float offsetX = getOffset(x, step);
    float offsetY = getOffset(y, step);
    model = translate(model, glm::vec3(offsetX, 0, offsetY));

    glUniformMatrix4fv(glGetUniformLocation(shader_->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    objects_[index].draw();
}

void Application::keyCallback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        onOff_ = !onOff_;
    }

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            camera_->processMoveStatus(CameraMovement::FORWARD, true);
            break;
        case GLFW_KEY_S:
            camera_->processMoveStatus(CameraMovement::BACKWARD, true);
            break;
        case GLFW_KEY_A:
            camera_->processMoveStatus(CameraMovement::LEFT, true);
            break;
        case GLFW_KEY_D:
            camera_->processMoveStatus(CameraMovement::RIGHT, true);
            break;
        case GLFW_KEY_UP:
            camera_->processMoveStatus(CameraMovement::UP, true);
            break;
        case GLFW_KEY_DOWN:
            camera_->processMoveStatus(CameraMovement::DOWN, true);
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            camera_->processMoveStatus(CameraMovement::FORWARD, false);
            break;
        case GLFW_KEY_S:
            camera_->processMoveStatus(CameraMovement::BACKWARD, false);
            break;
        case GLFW_KEY_A:
            camera_->processMoveStatus(CameraMovement::LEFT, false);
            break;
        case GLFW_KEY_D:
            camera_->processMoveStatus(CameraMovement::RIGHT, false);
            break;
        case GLFW_KEY_UP:
            camera_->processMoveStatus(CameraMovement::UP, false);
            break;
        case GLFW_KEY_DOWN:
            camera_->processMoveStatus(CameraMovement::DOWN, false);
            break;
        default:
            break;
        }
    }
}

int Application::width() const
{
    return width_;
}

int Application::height() const
{
    return height_;
}
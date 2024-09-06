
#include "sandbox_layer.h"
#include "GLFW/glfw3.h"
#include "core/app/application.h"
#include "core/log/log.h"
#include "imgui.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

SandboxLayer::SandboxLayer() {}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {
    glfwSetFramebufferSizeCallback((GLFWwindow*)CG::Application::Get().GetWindow().GetNativeWindow(),
                                   framebuffer_size_callback);

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource_, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success {};
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        CG_APP_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{0}", infoLog);
    }
    // fragmeng shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource_, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        CG_APP_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{0}", infoLog);
    }
    // link shaders
    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);
    // check for linking errors
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_, 512, nullptr, infoLog);
        CG_APP_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{0}", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glCreateVertexArrays(1, &VAO_);
    glCreateBuffers(1, &VBO_);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not
    // directly necessary.
    glBindVertexArray(0);
}

void SandboxLayer::OnDetach() {
    // Shutdown here
}

void SandboxLayer::OnEvent(CG::Event& event) {
    // Events here
}

void SandboxLayer::OnUpdate(CG::Timestep ts) {
    // Render here
    glClearColor(background_color_[0], background_color_[1], background_color_[2], background_color_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, wireMode_ ? GL_LINE : GL_FILL);
    // draw our first triangle
    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_); // seeing as we only have a single VAO there's no need to bind it every time, but we'll
                             // do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time
}

void SandboxLayer::OnImGuiRender() {
    // ImGui here
    ImGui::ColorEdit4("背景颜色", background_color_);
    ImGui::Checkbox("线框模式", &wireMode_);
}

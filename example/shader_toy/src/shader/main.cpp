#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <wtr/watcher.hpp>

using namespace std;
using namespace wtr;

#include "log/log.h"
#include "shader/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

glm::vec2 iMouse;

std::unique_ptr<Shader> shader;

std::mutex mut;

auto show(event e) {
    cout << to<string>(e.effect_type) + ' ' + to<string>(e.path_type) + ' ' + to<string>(e.path_name) +
                (e.associated ? " -> " + to<string>(e.associated->path_name) : "")
         << endl;
    auto re = e.effect_type;
    // if (re == wtr::watcher::event::effect_type::modify) {
    //
    auto t = std::thread([]() {
        std::lock_guard<std::mutex> lock(mut);
        // shader.reset(new Shader { "shader/quad.vs", "shader/quad.fs" });
        shader = std::make_unique<Shader>("shader/quad.vs", "shader/quad.fs");
    });
    t.join();
    // }
}

constexpr float iFrame {};

// init the shader by the shader class

int main(int argc, char* argv[]) {

    // Do some work. (We'll just wait for a newline.)

    Logger::init();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shader Toy", nullptr, nullptr);
    if (window == nullptr) {
        // std::cerr << "Failed to create GLFW window" << std::endl;
        ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, [](GLFWwindow*, double xpos, double ypos) {
        iMouse.x = xpos;
        iMouse.y = SCR_HEIGHT - ypos;
        // INFO("xpos: {}, ypos: {}", xpos, ypos);
        // INFO("iMouse.x: {}, iMouse.y: {}", iMouse.x, iMouse.y);
    });

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        // std::cerr << "Failed to initialize GLAD" << std::endl;
        ERROR("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    //
    // Shader shader("shader/quad.vs", "shader/quad.fs");
    shader = std::make_unique<Shader>("shader/quad.vs", "shader/quad.fs");

    // auto watcher = watch("./shader", show);

    GLuint VAO {}, VBO {};
    // create a plane vertex with four pos
    float vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };

    // set the vao and vbo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the vao and vbo
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // set the data fo vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // set the vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the vao and vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        shader = std::make_unique<Shader>("shader/quad.vs", "shader/quad.fs");

        // std::lock_guard<std::mutex> lock(mut);
        // process the input
        processInput(window);
        // set the color of the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // clear the color buffer and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use the shader
        shader->bind();
        // set the iResolution
        // create a vec2 to store the resolution
        glm::vec2 resolution;
        resolution.x = SCR_WIDTH;
        resolution.y = SCR_HEIGHT;
        // set the iResolution
        shader->setVec2("iResolution", resolution);
        // set the iTime
        shader->setFloat("iTime", glfwGetTime());
        shader->setVec2("iMouse", iMouse);
        shader->setFloat("iFrame", iFrame);

        // bind the vao
        glBindVertexArray(VAO);
        // draw the rectangle
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // unbind the vao
        glBindVertexArray(0);

        // swap the buffer
        glfwSwapBuffers(window);
        // poll the event
        glfwPollEvents();
    }

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

/*
着色器输入
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform float     iFrameRate;            // shader frame rate
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
*/

#include <iostream>
#include <string>
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

#include "log/log.h"
#include "shader/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

glm::vec2 iMouse;

int main(int argc, char* argv[]) {

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
        iMouse.y = ypos;
    });

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        // std::cerr << "Failed to initialize GLAD" << std::endl;
        ERROR("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    GLuint VAO {}, VBO {};
    // create a plane vertex with four pos
    float vertices[] = { 
        -1.0f, -1.0f,
         1.0f, -1.0f, 
         -1.0f, 1.0f, 
         1.0f, 1.0f 
         };

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

    // init the shader by the shader class
    Shader shader("shader/quad.vs", "shader/quad.fs");

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // process the input
        processInput(window);
        // set the color of the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // clear the color buffer and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use the shader
        shader.bind();
        // set the iResolution
        // create a vec2 to store the resolution
        glm::vec2 resolution;
        resolution.x = SCR_WIDTH;
        resolution.y = SCR_HEIGHT;
        // set the iResolution
        shader.setVec2("iResolution", resolution);
        // set the iTime
        shader.setFloat("iTime", glfwGetTime());
        shader.setVec2("iMouse", iMouse);

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

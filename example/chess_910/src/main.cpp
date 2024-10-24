// add #define GLEW_STATIC
#define GLEW_STATIC

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "application.h"

#include <iostream>

using namespace std;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Application::instance().keyCallback(window, key, scancode, action, mods);
}

int main()
{
    Application &app = Application::instance();

    if (glfwInit() == GLFW_FALSE)
    {
        cout << "Initialize glfw error" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(app.width(), app.height(), "CHESS", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "CreateWindow error" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        cout << "GlewInit error" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, keyCallback);

    app.init(window);

    while (GL_TRUE != glfwWindowShouldClose(window))
    {
        app.display(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

    // if (!glfwInit())
    // {
    //     fprintf(stderr, "Failed to initialize GLFW\n");
    //     getchar();
    //     return -1;
    // }

    // // Ensure we can capture the escape key being pressed below
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // // Hide the mouse and enable unlimited movement
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // // Set the mouse at the center of the screen
    // glfwPollEvents();
    // glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // // Dark blue background
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // // Enable depth test
    // glEnable(GL_DEPTH_TEST);
    // // Accept fragment if it is closer to the camera than the former one
    // glDepthFunc(GL_LESS);

    // // Cull triangles which normal is not towards the camera
    // glEnable(GL_CULL_FACE);

    // // Create and compile our GLSL program from the shaders
    // GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

    // // Get a handle for our "MVP" uniform
    // GLint MatrixID = glGetUniformLocation(programID, "MVP");
    // GLint ViewMatrixID = glGetUniformLocation(programID, "V");
    // GLint ModelMatrixID = glGetUniformLocation(programID, "M");

    // // Get a handle for our "myTextureSampler" uniform
    // GLint TextureID = glGetUniformLocation(programID, "myTextureSampler");
    // // Set our "myTextureSampler" sampler to use Texture Unit 0
    // glUniform1i(TextureID, 0);

    // // Get a handle for our "LightPosition" uniform
    // GLint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // // Read our .obj file
    // ChessModel *chessModel = new ChessModel();
    // chessModel->loadModel("Stone_Chess_Board/12951_Stone_Chess_Board_v1_L3.obj");
    // chessModel->loadModel("Chess/chess.obj");

    // // For speed computation
    // double lastTime = glfwGetTime();
    // int nbFrames = 0;

    // do
    // {
    //     // Measure speed
    //     double currentTime = glfwGetTime();
    //     nbFrames++;
    //     if (currentTime - lastTime >= 1.0)
    //     { // If last prinf() was more than 1sec ago
    //         // printf and reset
    //         printf("%f ms/frame\n", 1000.0 / double(nbFrames));
    //         nbFrames = 0;
    //         lastTime += 1.0;
    //     }

    //     // Clear the screen
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //     // Use our shader
    //     glUseProgram(programID);

    //     // Compute the MVP matrix from keyboard and mouse input
    //     computeMatricesFromInputs();

    //     vec3 lightPos = vec3(4, 4, 4);
    //     glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

    //     mat4 ProjectionMatrix = getProjectionMatrix();
    //     mat4 ViewMatrix = getViewMatrix();
    //     glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

    //     // draw board
    //     mat4 ModelMatrix = mat4(1.0);
    //     ModelMatrix = scale(ModelMatrix, vec3(0.1));
    //     ModelMatrix = translate(ModelMatrix, vec3(0, 0, 0));
    //     mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    //     glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    //     glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

    //     chessModel->draw(0);

    //     // draw chess 1
    //     drawChess(chessModel, 1, {0, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {1, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {2, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {3, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {4, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {5, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {6, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 1, {7, 1}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 2, {0, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 2, {7, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 3, {1, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 3, {6, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 4, {2, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 4, {5, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 5, {3, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 6, {4, 0}, true, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {0, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {1, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {2, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {3, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {4, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {5, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {6, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 7, {7, 6}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 8, {0, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 8, {7, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 9, {1, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 9, {6, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 10, {2, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 10, {5, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 11, {3, 7}, false, MatrixID, ModelMatrixID);
    //     drawChess(chessModel, 12, {4, 7}, false, MatrixID, ModelMatrixID);

    //     // Swap buffers
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();

    // } // Check if the ESC key was pressed or the window was closed
    // while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    //     ;

    // // Cleanup VBO and shader
    // glDeleteProgram(programID);

    // // Close OpenGL window and terminate GLFW
    // glfwTerminate();

    // return 0;
}

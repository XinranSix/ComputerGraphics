#define SDL_MAIN_HANDLED

// #include <SDL2/SDL.h>
#include <SDL.h>
#include <glad/glad.h>

void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置背景色
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

    glBegin(GL_TRIANGLES); // 开始绘制三角形
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f); // 顶点1
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f); // 顶点2
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.5f, -0.5f); // 顶点3
    glEnd();
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // 初始化SDL
    SDL_Window* window = SDL_CreateWindow("SDL + GLAD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_GLContext glContext = SDL_GL_CreateContext(window); // 创建OpenGL上下文
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);  // 加载GLAD

    initialize(); // 初始化OpenGL

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // 处理退出事件
            }
        }
        render();                  // 渲染
        SDL_GL_SwapWindow(window); // 交换缓冲区
    }

    SDL_GL_DeleteContext(glContext); // 删除OpenGL上下文
    SDL_DestroyWindow(window);       // 销毁窗口
    SDL_Quit();                      // 退出SDL
    return 0;
}

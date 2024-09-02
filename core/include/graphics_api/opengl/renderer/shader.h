#pragma once

#include <string>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace CG::GL {

    class Shader {
    public:
        ~Shader();

        GLuint GetRendererID() { return rendererID_; }

        static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:
        Shader() = default;

        void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        GLuint CompileShader(GLenum type, const std::string& source);

    private:
        GLuint rendererID_;
    };

} // namespace CG::GL

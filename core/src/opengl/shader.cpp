#include <fstream>

#include "core/log.h"
#include "opengl/shader.h"

namespace CG::Utils {

    static std::string ReadFileAsString(const std::string& filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in );
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            result.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        } else {
            CG_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    Shader::~Shader() { glDeleteProgram(rendererID_); }

    GLuint Shader::CompileShader(GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            CG_CORE_ERROR("{0}", infoLog.data());
            // CG_CORE_ASSERT(false, "Shader compilation failure!");
        }

        return shader;
    }

    Shader* Shader::FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
        Shader* shader = new Shader();
        shader->LoadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath);
        return shader;
    }

    void Shader::LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
        std::string vertexSource = ReadFileAsString(vertexShaderPath);
        std::string fragmentSource = ReadFileAsString(fragmentShaderPath);

        GLuint program = glCreateProgram();
        int glShaderIDIndex = 0;

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            CG_CORE_ERROR("{0}", infoLog.data());
            // CG_CORE_ASSERT(false, "Shader link failure!");
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        rendererID_ = program;
    }

} // namespace CG::Utils
// example/opengl_example/assets/shader/test.frag.glsl

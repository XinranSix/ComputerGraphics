#include <filesystem>
#include <fstream>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/gtc/type_ptr.hpp>

#include "core/log/log.h"
#include "core/timer.h"
#include "graphics_api/opengl/renderer/opengl_shader.h"

namespace CG {
    namespace Utils {
        static GLenum ShaderTypeFromString(const std::string& type) {
            if (type == "vertex") {
                return GL_VERTEX_SHADER;
            }
            if (type == "fragment" || type == "pixel") {
                return GL_FRAGMENT_SHADER;
            }
            CG_CORE_ASSERT(false, "Unknown shader type!F");
            return 0;
        }

    } // namespace Utils

    OpenGLShader::OpenGLShader(const std::string& filepath) : filePath_ { filepath } {
        // CG_PROFILE_FUNCTION();
        std::string source = ReadFile(filepath);
        CG_CORE_ASSERT(source.size(), "Source is empty!");
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        name_ = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : name_ { name } {
        // CG_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader() {
        // CG_PROFILE_FUNCTION();
        glDeleteProgram(rendererID_);
    }
    std::string OpenGLShader::ReadFile(const std::string& filepath) {
        // CG_PROFILE_FUNCTION();
        std::string result;
        std::ifstream in { filepath, std::ios::in | std::ios::binary };
        if (in) {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1) {
                result.resize(in.tellg());
                in.seekg(0, std::ios::beg);
                in.read(&result[0], result.size());
            } else {
                CG_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        } else {
            CG_CORE_ERROR("Could open file '{0}'", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
        // CG_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> shaderSources;

        std::string typeToken = "#type";
        size_t typeTokenLen = typeToken.size();
        size_t findCurPos = source.find(typeToken, 0);
        size_t findNextPos = findCurPos;
        while (findNextPos != std::string::npos) {
            size_t curlineEndPos = source.find_first_of("\r\n", findCurPos); /// r/nÐ´´íÎª/r/n
            CG_CORE_ASSERT((curlineEndPos != std::string::npos), "Error");
            size_t begin = findCurPos + typeTokenLen + 1;

            std::string type = source.substr(begin, curlineEndPos - begin); // »ñÈ¡µ½ÊÇvertex»¹ÊÇfragment
            CG_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Error");

            size_t nextLinePos = source.find_first_not_of("\r\n", curlineEndPos);
            findNextPos = source.find(typeToken, nextLinePos);
            shaderSources[Utils::ShaderTypeFromString(type)] = source.substr(
                nextLinePos, findNextPos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

            findCurPos = findNextPos;
        }
        return shaderSources;
    }

    void OpenGLShader::CreateProgram() {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        for (auto&& [stage, spirv] : openglSPIRV_) {
            GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
            glAttachShader(program, shaderID);
        }
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            CG_CORE_ERROR("Shader linking failed ({0}):\n{1}", filePath_, infoLog.data());

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            for (auto id : shaderIDs) {
                glDeleteShader(id);
            }
            // Use the infoLog as you see fit.
            // In this simple program, we'll just leave
            CG_CORE_ERROR("{0} ", infoLog.data());
            CG_CORE_ASSERT(false, "shader link failure!");
        }

        // Always detach shaders after a successful link.
        for (auto id : shaderIDs) {
            glDetachShader(program, id);
            glDeleteShader(id);
        }
        rendererID_ = program;
    }

    void OpenGLShader::Bind() const {
        //  CG_PROFILE_FUNCTION();
        glUseProgram(rendererID_);
    }

    void OpenGLShader::Unbind() const {
        //  CG_PROFILE_FUNCTION();
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value) {
        //  CG_PROFILE_FUNCTION();
        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
        UploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) {
        //  CG_PROFILE_FUNCTION();
        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
        //  CG_PROFILE_FUNCTION();
        UploadUniformFloat2(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
        //  CG_PROFILE_FUNCTION();
        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
        //  CG_PROFILE_FUNCTION();
        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
        //  CG_PROFILE_FUNCTION();
        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
        GLint location = glGetUniformLocation(rendererID_, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    // namespace Monado

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
        // CG_PROFILE_FUNCTION();

        GLuint program = glCreateProgram();
        /*
                std::vector<GLenum> glShaderIDs(shaderSources.size());
                glShaderIDs.reserve(2);
        */

        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources) {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            // Create an empty vertex shader handle
            GLuint shader = glCreateShader(type);

            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            // Compile the vertex shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                // Use the infoLog as you see fit.

                // In this simple program, we'll just leave
                CG_CORE_ERROR("{0} ", infoLog.data());
                CG_CORE_ASSERT(false, "shader ±àÒëÊ§°Ü!");

                break;
            }

            // Attach our shaders to our program
            glAttachShader(program, shader);

            // glShaderIDs.push_back(shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }
        //
        rendererID_ = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            for (auto id : glShaderIDs) {
                glDeleteShader(id);
            }
            // Use the infoLog as you see fit.
            // In this simple program, we'll just leave
            CG_CORE_ERROR("{0} ", infoLog.data());
            CG_CORE_ASSERT(false, "shader link failure!");
            return;
        }

        // Always detach shaders after a successful link.
        for (auto id : glShaderIDs) {
            glDetachShader(program, id);
        }
    }
} // namespace CG

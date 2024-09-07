#pragma once

#include <string>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "renderer/shader.h"

namespace CG {
    class OpenGLShader : public Shader {
    public:
        // 从文件中读取
        OpenGLShader(const std::string& filepath);
        // 从字符串中读取
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        // 设置 uniform 变量
        virtual void SetInt(const std::string& name, int value) override;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

        virtual const std::string& GetName() const override { return name_; };

        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        // 读取文件
        std::string ReadFile(const std::string& filepath);
        // 预处理
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        // void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string> &shaderSources);
        // void CompileOrGetOpenGLBinaries();
        // 创建 Shader Program
        void CreateProgram();
        // void Reflect(GLenum stage, const std::vector<uint32_t> &shaderData);

        // 编译
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    private:
        uint32_t rendererID_;
        std::string filePath_;
        std::string name_;

        std::unordered_map<GLenum, std::vector<uint32_t>> openglSPIRV_;
        std::unordered_map<GLenum, std::string> openglLSourceCode_;
    };
} // namespace CG

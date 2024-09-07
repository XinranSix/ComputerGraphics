// #include <format>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "renderer/shader.h"
#include "core/base/base.h"
#include "graphics_api/opengl/renderer/opengl_shader.h"

namespace CG {

    Ref<Shader> Shader::Create(const std::string& filepath) { return CreateRef<OpenGLShader>(filepath); }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
        return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    //////////////////////////////////////////////////////////////////
    // ShaderLibrary//////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    void ShaderLibrary::Add(const Ref<Shader>& shader) {
        auto& name = shader->GetName();
        Add(name, shader);
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
        CG_CORE_ASSERT(!Exists(name), std::format("Shader [{}] is already existed!", name).c_str());
        shaders_[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name) {
        CG_CORE_ASSERT(Exists(name), std::format("Shader [{}] dose not exists", name).c_str());
        return shaders_[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const { return shaders_.find(name) != shaders_.end(); }
} // namespace CG

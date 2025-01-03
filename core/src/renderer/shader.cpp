// #include <format>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "renderer/shader.h"
#include "core/base/base.h"
#include "graphics_api/opengl/renderer/opengl_shader.h"
#include "renderer/renderer.h"
#include "renderer/renderer_api.h"

namespace CG {

    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                CG_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
        }
        CG_CORE_ASSERT(false, "UnKnown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                CG_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
        CG_CORE_ASSERT(false, "0x1908UnKnown RendererAPI!");
        return nullptr;
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

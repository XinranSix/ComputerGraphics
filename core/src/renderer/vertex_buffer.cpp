#pragma once

#include "renderer/vertex_buffer.h"
#include "graphics_api/opengl/renderer/opengl_vertex_buffer.h"
#include "renderer/renderer.h"

namespace CG {

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                CG_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
        }
        CG_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                CG_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }
        CG_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace CG

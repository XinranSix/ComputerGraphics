#include "renderer/index_buffer.h"
#include "graphics_api/opengl/renderer/opengl_index_buffer.h"
#include "renderer/renderer.h"

namespace CG {

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                CG_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
        }
        CG_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace CG

#include "renderer/vertex_array.h"
#include "graphics_api/opengl/renderer/opengl_vertex_array.h"
#include "renderer/renderer.h"

namespace CG {

    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                CG_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
        }
        CG_CORE_ASSERT(false, "UnKnown RendererAPI!");
        return nullptr;
    }
} // namespace CG

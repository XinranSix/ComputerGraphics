// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "graphics_api/opengl/renderer/opengl_vertex_buffer.h"

namespace CG {

    /////////////////////////////////////////////////////////////////////////////
    // VertexBuffer /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
        // CG_PROFILE_FUNCTION();
        glCreateBuffers(1, &rendererID_);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
        // CG_PROFILE_FUNCTION();
        glCreateBuffers(1, &rendererID_);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        // CG_PROFILE_FUNCTION();
        glDeleteBuffers(1, &rendererID_);
    }

    void OpenGLVertexBuffer::Bind() const {
        // CG_PROFILE_FUNCTION();
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
    }

    void OpenGLVertexBuffer::Unbind() const {
        // CG_PROFILE_FUNCTION();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

} // namespace CG

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "graphics_api/opengl/renderer/opengl_index_buffer.h"

namespace CG {
    /////////////////////////////////////////////////////////////////////////////
    // IndexBuffer //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : count_ { count } {
        // CG_PROFILE_FUNCTION();
        glCreateBuffers(1, &rendererID_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        // CG_PROFILE_FUNCTION();
        glDeleteBuffers(1, &rendererID_);
    }

    void OpenGLIndexBuffer::Bind() const {
        // CG_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
    }

    void OpenGLIndexBuffer::Unbind() const {
        // CG_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
} // namespace CG

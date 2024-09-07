#pragma once

#include "renderer/index_buffer.h"

namespace CG {

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;

        virtual uint32_t GetCount() const { return count_; }

    private:
        uint32_t rendererID_;
        uint32_t count_;
    };
} // namespace CG

#pragma once

#include "renderer/vertex_buffer.h"

namespace CG {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual const BufferLayout& GetLayout() const override { return layout_; }
        virtual void SetLayout(const BufferLayout& layout) override { layout_ = layout; }

    private:
        uint32_t rendererID_;
        BufferLayout layout_;
    };

} // namespace CG

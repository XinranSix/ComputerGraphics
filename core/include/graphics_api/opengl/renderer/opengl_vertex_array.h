#pragma once

#include "renderer/vertex_array.h"

namespace CG {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return vertexBuffers_; };
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer_; }

    private:
        uint32_t vertexBufferIndex_ = 0;
        uint32_t rendererID_;
        std::vector<Ref<VertexBuffer>> vertexBuffers_;
        Ref<IndexBuffer> indexBuffer_;
    };

} // namespace CG

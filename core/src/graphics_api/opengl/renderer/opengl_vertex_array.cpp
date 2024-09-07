// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "graphics_api/opengl/renderer/opengl_vertex_array.h"
#include "renderer/vertex_buffer.h"

namespace CG {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3: return GL_FLOAT;
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
            default: break;
        }
        CG_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        // CG_PROFILE_FUNCTION();
        glCreateVertexArrays(1, &rendererID_);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        // CG_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &rendererID_);
    }

    void OpenGLVertexArray::Bind() const {
        // CG_PROFILE_FUNCTION();
        glBindVertexArray(rendererID_);
    }

    void OpenGLVertexArray::Unbind() const {
        // CG_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        // CG_PROFILE_FUNCTION();

        CG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout;!");

        glBindVertexArray(rendererID_);

        vertexBuffer->Bind();

        // uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout) {
            switch (element.type) {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4: {
                    glEnableVertexAttribArray(vertexBufferIndex_);
                    glVertexAttribPointer(
                        vertexBufferIndex_, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                        (const void*)element.offset); // ָ�����֡�define an array of generic vertex attribute data
                    vertexBufferIndex_++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool: {
                    // glVertexAttribIPointer  glVertexAttribPointer
                    glEnableVertexAttribArray(vertexBufferIndex_);
                    glVertexAttribIPointer(
                        vertexBufferIndex_, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
                        layout.GetStride(),
                        (const void*)element.offset); // ָ�����֡�define an array of generic vertex attribute data
                    vertexBufferIndex_++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4: {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++) {
                        glEnableVertexAttribArray(vertexBufferIndex_);
                        glVertexAttribPointer(
                            vertexBufferIndex_, count, ShaderDataTypeToOpenGLBaseType(element.type),
                            element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                            (const void*)(element.offset +
                                          sizeof(float) * count *
                                              i)); // ָ�����֡�define an array of generic vertex attribute data
                        glVertexAttribDivisor(vertexBufferIndex_, 1);
                        vertexBufferIndex_++;
                    }
                    break;
                }
                default: CG_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }
        vertexBuffers_.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        // CG_PROFILE_FUNCTION();

        glBindVertexArray(rendererID_);
        indexBuffer->Bind();
        indexBuffer_ = indexBuffer;
    }
} // namespace CG

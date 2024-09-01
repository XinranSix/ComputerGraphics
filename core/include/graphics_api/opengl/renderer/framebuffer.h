#pragma once

#include <memory>
#include <vector>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/glm.hpp>

namespace CG::GL {

    // 帧缓冲纹理格式，包括颜色附件、深度附件和模板附件
    enum class FramebufferTextureFormat {
        None = 0,

        // Color
        RGBA8 = 1,
        RGBA16F = 2,
        RGBA32F = 3,
        RG32F = 4,

        // Depth/stencil
        DEPTH32F = 5,
        DEPTH24STENCIL8 = 6,

        // Defaults
        Depth = DEPTH24STENCIL8
    };

    // 创建缓冲区要指定的格式
    struct FramebufferTextureSpecification {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format) : textureFormat { format } {}

        FramebufferTextureFormat textureFormat;
        // TODO: filtering/wrap
    };

    // 缓冲区附加到帧缓冲中要指定的格式 - 可以认为只是多一层封装 FramebufferTextureSpecification 成 vector
    struct FramebufferAttachmentSpecification {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
            : attachments { attachments } {}

        std::vector<FramebufferTextureSpecification> attachments;
    };

    struct FramebufferSpecification {
        uint32_t width = 1280;
        uint32_t height = 720;
        glm::vec4 clearColor;
        FramebufferAttachmentSpecification attachments;
        uint32_t samples = 1; // multisampling

        // TODO: Temp, needs scale
        bool noResize { false };

        // SwapChainTarget = screen buffer (i.e. no framebuffer)
        bool swapChainTarget = { false };
    };

    class Framebuffer {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        void Resize(uint32_t width, uint32_t height, bool forceRecreate = false);
        int ReadPixel(uint32_t attachmentIndex, int x, int y);

        void Bind();
        void Unbind();

        void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const;

        uint32_t GetWidth() const { return specification_.width; };
        uint32_t GetHeight() const { return specification_.height; };

        GLuint GetRendererID() const { return rendererID_; }
        GLuint GetColorAttachmentRendererID(int index = 0) const { return colorAttachments_[index]; }
        GLuint GetDepthAttachmentRendererID() const { return depthAttachment_; }

        const FramebufferSpecification& GetSpecification() { return specification_; }

        static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);

    private:
        FramebufferSpecification specification_;
        GLuint rendererID_ {};

        std::vector<GLuint> colorAttachments_;
        GLuint depthAttachment_;

        std::vector<FramebufferTextureFormat> colorAttachmentFormats_;
        FramebufferTextureFormat depthAttachmentFormat_ = FramebufferTextureFormat::None;

        uint32_t width_ {}, height_ {};
    };

    class FramebufferPool final {
    public:
        FramebufferPool(uint32_t maxFBs = 32); // ?
        ~FramebufferPool();

        std::weak_ptr<Framebuffer> AllocateBuffer();
        void Add(const std::shared_ptr<Framebuffer>& framebuffer);

        std::vector<std::shared_ptr<Framebuffer>>& GetAll() { return pool_; }
        const std::vector<std::shared_ptr<Framebuffer>>& GetAll() const { return pool_; }

        inline static FramebufferPool* GetGlobal() { return sInstance_; }

    private:
        std::vector<std::shared_ptr<Framebuffer>> pool_;
        static FramebufferPool* sInstance_;
    };

} // namespace CG::GL

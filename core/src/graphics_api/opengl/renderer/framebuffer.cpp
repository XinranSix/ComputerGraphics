#include <memory>

#include "core/base/base.h"
#include "graphics_api/opengl/renderer/framebuffer.h"

namespace CG::GL {

    namespace Utils {
        static GLenum TextureTarget(bool multisampled) {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multisampled, GLuint* outID, uint32_t count) {
            glCreateTextures(TextureTarget(multisampled), 1, outID);
        }

        static void BindTexture(bool multisampled, GLuint id) { glBindTexture(TextureTarget(multisampled), id); }

        static GLenum DataType(GLenum format) {
            switch (format) {
                case GL_RGBA8: return GL_UNSIGNED_BYTE;
                case GL_RG16F:
                case GL_RG32F:
                case GL_RGBA16F:
                case GL_RGBA32F: return GL_FLOAT;
                case GL_DEPTH24_STENCIL8: return GL_UNSIGNED_INT_24_8;
            }

            CG_CORE_ASSERT(false, "Unknown format!");
            return 0;
        }

        static void AttachColorTexture(GLuint id, int samples, GLenum format, uint32_t width, uint32_t height,
                                       int index) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            } else {
                // Only RGBA access for now
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, DataType(format), nullptr);

                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(GLuint id, int samples, GLenum format, GLenum attachmentType, uint32_t width,
                                       uint32_t height) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            } else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                case FramebufferTextureFormat::DEPTH32F: return true;
                default: break;
            }
            return false;
        }

    } // namespace Utils

    Framebuffer::Framebuffer(const FramebufferSpecification& spec)
        : specification_ { spec }, width_ { spec.width }, height_ { spec.height } {
        for (auto format : specification_.attachments.attachments) {
            if (!Utils::IsDepthFormat(format.textureFormat)) {
                colorAttachmentFormats_.emplace_back(format.textureFormat);
            } else {
                depthAttachmentFormat_ = format.textureFormat;
            }
        }
    }

    Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &rendererID_); }

    void Framebuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate) {
        if (!forceRecreate && (width_ == width && height_ == height)) {
            return;
        }

        width_ = width;
        height_ = height;

        if (rendererID_ != 0) {
            glDeleteFramebuffers(1, &rendererID_);
            glDeleteTextures(colorAttachments_.size(), colorAttachments_.data());
            glDeleteTextures(1, &depthAttachment_);

            colorAttachments_.clear();
            depthAttachment_ = 0;
        }

        glGenFramebuffers(1, &rendererID_);
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID_);

        bool multisample = specification_.samples > 1;

        if (colorAttachmentFormats_.size()) {
            colorAttachments_.resize(colorAttachmentFormats_.size());
            Utils::CreateTextures(multisample, colorAttachments_.data(), colorAttachments_.size());

            // Create color attachments
            for (int i = 0; i < colorAttachments_.size(); i++) {
                Utils::BindTexture(multisample, colorAttachments_[i]);
                switch (colorAttachmentFormats_[i]) {
                    case FramebufferTextureFormat::RGBA8:
                        Utils::AttachColorTexture(colorAttachments_[i], specification_.samples, GL_RGBA8, width_,
                                                  height_, i);
                        break;
                    case FramebufferTextureFormat::RGBA16F:
                        Utils::AttachColorTexture(colorAttachments_[i], specification_.samples, GL_RGBA16F, width_,
                                                  height_, i);
                        break;
                    case FramebufferTextureFormat::RGBA32F:
                        Utils::AttachColorTexture(colorAttachments_[i], specification_.samples, GL_RGBA32F, width_,
                                                  height_, i);
                        break;
                    case FramebufferTextureFormat::RG32F:
                        Utils::AttachColorTexture(colorAttachments_[i], specification_.samples, GL_RG32F, width_,
                                                  height_, i);
                        break;
                }
            }
        }

        if (depthAttachmentFormat_ != FramebufferTextureFormat::None) {
            Utils::CreateTextures(multisample, &depthAttachment_, 1);
            Utils::BindTexture(multisample, depthAttachment_);
            switch (depthAttachmentFormat_) {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(depthAttachment_, specification_.samples, GL_DEPTH24_STENCIL8,
                                              GL_DEPTH_STENCIL_ATTACHMENT, width_, height_);
                    break;
                case FramebufferTextureFormat::DEPTH32F:
                    Utils::AttachDepthTexture(depthAttachment_, specification_.samples, GL_DEPTH_COMPONENT32F,
                                              GL_DEPTH_ATTACHMENT, width_, height_);
                    break;
            }
        }

        if (colorAttachments_.size() > 1) {

            CG_CORE_ASSERT(colorAttachments_.size() <= 4, "");

            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                  GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(colorAttachments_.size(), buffers);
        } else if (colorAttachments_.size() == 0) {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

        CG_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                       "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        CG_CORE_ASSERT((attachmentIndex < colorAttachments_.size()), "");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID_);
        glViewport(0, 0, width_, height_);
    }

    void Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void Framebuffer::BindTexture(uint32_t attachmentIndex, uint32_t slot) const {
        glBindTextureUnit(slot, colorAttachments_[attachmentIndex]);
    }

    std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
        return std::make_shared<Framebuffer>(spec);
    }

} // namespace CG::GL

namespace CG::GL {

    static FramebufferPool* sInstance_ = new FramebufferPool();

    FramebufferPool::FramebufferPool(uint32_t maxFbs) {}

    FramebufferPool::~FramebufferPool() {}

    std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer() {
        // m_Pool.push_back();
        return std::weak_ptr<Framebuffer>();
    }

    void FramebufferPool::Add(const std::shared_ptr<Framebuffer>& framebuffer) { pool_.push_back(framebuffer); }

} // namespace CG::GL

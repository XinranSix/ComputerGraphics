#pragma once

#include <string>
#include <string_view>

namespace CG {

    class Image {

        enum class Format { None = 0, RGBA, RGBA32F };

    public:
        Image(std::string_view path);

        Image(uint32_t width, uint32_t height, Format format, const void* data = nullptr);

        ~Image();

        void SetData(const void* data);

        // VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }

        void Resize(uint32_t width, uint32_t height);

        inline uint32_t GetWidth() const { return width_; }
        inline uint32_t GetHeight() const { return height_; }

    private:
        // void AllocateMemory(uint64_t size);
        // void Release();

    private:
        uint32_t width_ {};
        uint32_t height_ {};

        std::byte* data_ {};

        std::string filepath_;
    };
} // namespace CG

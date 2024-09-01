#pragma once

#include <cstddef>
#include <filesystem>

namespace CG {

    class Image {

    public:
        Image(const std::filesystem::path& path);

        Image(uint32_t width = 0, uint32_t height = 0, uint32_t channels = 0, const void* data = nullptr);

        Image(const Image& other);

        Image& operator=(const Image& rhs);

        Image(Image&& other) noexcept;

        Image& operator=(Image&& rhs) noexcept;

        ~Image() noexcept;

        void SetData(uint32_t width = 0, uint32_t height = 0, uint32_t channels = 0, const void* data = nullptr);

        void Resize(uint32_t width, uint32_t height);

        void SaveAsPNG(std::filesystem::path path = "output.png");
        void SaveAsBMP(std::filesystem::path path = "output.bmp");
        void SaveAsTGA(std::filesystem::path path = "output.tga");
        void SaveAsJPG(std::filesystem::path path = "output.jpg");

        inline uint32_t GetWidth() const { return width_; }
        inline uint32_t GetHeight() const { return height_; }
        inline uint32_t GetChannels() const { return channels_; }
        inline std::byte* GetData() const { return data_; }

        friend void swap(Image& first, Image& second) noexcept { first.swap(second); }

    private:
        void swap(Image& other) noexcept {
            std::swap(width_, other.width_);
            std::swap(height_, other.width_);
            std::swap(channels_, other.channels_);
            std::swap(data_, other.data_);
        }

    private:
        uint32_t width_ {};
        uint32_t height_ {};
        uint32_t channels_ {};

        std::byte* data_ {};
    };
} // namespace CG

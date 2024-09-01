#include <algorithm>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "resources/image.h"

namespace CG {
    Image::Image(const std::filesystem::path& path) {
        int x {}, y {}, n {};
        unsigned char* data = stbi_load(path.string().c_str(), &x, &y, &n, 0);
        SetData(x, y, n, data);
        stbi_image_free(data);
    }

    Image::Image(uint32_t width, uint32_t height, uint32_t channels, const void* data) {
        SetData(width, height, channels, data);
    }

    Image::Image(const Image& other) { SetData(other.width_, other.height_, other.channels_, other.data_); }

    Image& Image::operator=(const Image& rhs) {
        Image tmp { rhs };
        swap(tmp);
        return *this;
    }

    Image::Image(Image&& other) noexcept { swap(other); }

    Image& Image::operator=(Image&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    Image::~Image() noexcept { SetData(); }

    void Image::SetData(uint32_t width, uint32_t height, uint32_t channels, const void* data) {
        delete[] data_;
        if (!data) {
            std::exchange(width_, 0);
            std::exchange(height_, 0);
            std::exchange(channels_, 0);
            std::exchange(data_, nullptr);
            return;
        }
        std::exchange(width_, width);
        std::exchange(height_, height);
        std::exchange(channels_, channels);
        data_ = new std::byte[width_ * height_ * channels_];
        std::copy(reinterpret_cast<const std::byte*>(data),
                  reinterpret_cast<const std::byte*>(data) + width_ * height_ * channels_, data_);
    }

    void Image::Resize(uint32_t width, uint32_t height) {
        std::byte* resized_data = new std::byte[width * height * channels_];
        stbir_resize_uint8_linear(reinterpret_cast<const unsigned char*>(data_), width_, height_, width_ * channels_,
                                  reinterpret_cast<unsigned char*>(resized_data), width, height, width * channels_,
                                  static_cast<stbir_pixel_layout>(channels_));
        SetData(width, height, channels_, resized_data);
    }

    void Image::SaveAsPNG(std::filesystem::path path) {
        stbi_write_png(path.string().c_str(), width_, height_, channels_, data_, width_ * channels_);
    }

    void Image::SaveAsBMP(std::filesystem::path path) {
        stbi_write_bmp(path.string().c_str(), width_, height_, channels_, data_);
    }

    void Image::SaveAsTGA(std::filesystem::path path) {
        stbi_write_tga(path.string().c_str(), width_, height_, channels_, data_);
    }

    void Image::SaveAsJPG(std::filesystem::path path) {
        stbi_write_jpg(path.string().c_str(), width_, height_, channels_, data_, 100);
    }

} // namespace CG

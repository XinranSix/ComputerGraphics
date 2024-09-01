#include <cstring>
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
        int x, y, n;
        unsigned char* data = stbi_load(path.string().c_str(), &x, &y, &n, 0);
        if (!data) {
            return;
        }
        std::exchange(width_, x);
        std::exchange(height_, y);
        std::exchange(channels_, n);
        data_ = new std::byte[width_ * height_ * channels_];
        memcpy(data_, data, width_ * height_ * channels_);
        stbi_image_free(data);
    }

    Image::Image(uint32_t width, uint32_t height, uint32_t channels, const void* data) {
        SetData(width, height, channels, data);
    }

    Image::Image(const Image& other) {
        std::exchange(width_, other.width_);
        std::exchange(height_, other.height_);
        std::exchange(channels_, other.channels_);
        data_ = new std::byte[width_ * height_ * channels_];
        memcpy(data_, other.data_, width_ * height_ * channels_);
    }

    Image& Image::operator=(const Image& rhs) {
        Image tmp(rhs);
        swap(tmp);
        return *this;
    }

    Image::Image(Image&& other) noexcept { swap(other); }

    Image& Image::operator=(Image&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    Image::~Image() noexcept {
        delete[] data_;
        std::exchange(width_, 0);
        std::exchange(height_, 0);
        std::exchange(channels_, 0);
        std::exchange(data_, nullptr);
    }

    void Image::SetData(uint32_t width, uint32_t height, uint32_t channels, const void* data) {
        if (!data) {
            return;
        }
        std::exchange(width_, width);
        std::exchange(height_, height);
        std::exchange(channels_, channels);
        delete[] data_;
        data_ = new std::byte[width_ * height_ * channels_];
        memcpy(data_, data, width_ * height_ * channels_);
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

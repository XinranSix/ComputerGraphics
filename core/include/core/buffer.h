#pragma once

#include <utility>

#include "core/base/assert.h"

namespace CosmoCore {

    class Buffer {
    public:
        Buffer() : Buffer { {}, {} } {}

        Buffer(std::byte* data, uint32_t size) : data_ { data }, size_ { size } {}

        Buffer(const Buffer& other) : size_ { other.size_ }, data_ { new std::byte[size_] } {
            memcpy(other.data_, other.data_, size_);
        }

        Buffer& operator=(const Buffer& rhs) {
            Buffer temp { rhs };
            swap(temp);
            return *this;
        }

        Buffer(Buffer&& other) { swap(other); }

        Buffer& operator=(Buffer&& rhs) {
            swap(rhs);
            return *this;
        }

        ~Buffer() noexcept {
            delete[] data_;
            std::exchange(size_, 0);
            std::exchange(data_, nullptr);
        }

        static Buffer Copy(void* data, uint32_t size) {
            Buffer buffer;
            buffer.Allocate(size);
            memcpy(buffer.data_, data, size);
            return buffer;
        }

        void Allocate(uint32_t size) {
            Buffer temp { new std::byte[size], size };
            swap(temp);
        }

        void ZeroInitialize() {
            if (data_) {
                memset(data_, 0, size_);
            }
        }

        template <typename T>
        T& Read(uint32_t offset = 0) {
            return *reinterpret_cast<T*>(data_ + offset);
        }

        void Write(void* data, uint32_t size, uint32_t offset = 0) {
            CG_CORE_ASSERT((offset + size <= size), "Buffer overflow!");
            memcpy(this->data_ + offset, data, size);
        }

        operator bool() const { return data_; }

        std::byte& operator[](int index) { return data_[index]; }

        std::byte operator[](int index) const { return data_[index]; }

        template <typename T>
        auto AS() {
            return reinterpret_cast<T*>(data_);
        }

        friend void swap(Buffer& first, Buffer& second) noexcept { first.swap(second); }

    private:
        void swap(Buffer& other) noexcept {
            std::swap(size_, other.size_);
            std::swap(data_, other.data_);
        }

    private:
        uint32_t size_;
        std::byte* data_;
    };

} // namespace CosmoCore

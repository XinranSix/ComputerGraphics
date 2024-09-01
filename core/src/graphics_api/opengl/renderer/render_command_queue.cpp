#include <cstddef>

#include "graphics_api/opengl/renderer/render_command_queue.h"

namespace CG {

    RenderCommandQueue::RenderCommandQueue() {
        commandBuffer_ = new uint8_t[10 * 1024 * 1024]; // 10mb buffer
        commandBufferPtr_ = commandBuffer_;
        memset(commandBuffer_, 0, 10 * 1024 * 1024);
    }

    RenderCommandQueue::~RenderCommandQueue() { delete[] commandBuffer_; }

    void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint32_t size) {
        // TODO: alignment
        *(RenderCommandFn*)commandBufferPtr_ = fn;
        commandBufferPtr_ += sizeof(RenderCommandFn);

        *(uint32_t*)commandBufferPtr_ = size;
        commandBufferPtr_ += sizeof(uint32_t);

        void* memory = commandBufferPtr_;
        commandBufferPtr_ += size;

        commandBufferPtr_++;
        return memory;
    }

    void RenderCommandQueue::Execute() {
        // CG_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", commandCount_,
        // (commandBufferPtr_
        // - commandBuffer_));

        std::byte* buffer = (std::byte*)commandBuffer_;

        for (uint32_t i = 0; i < commandCount_; i++) {
            RenderCommandFn function = *(RenderCommandFn*)buffer;
            buffer += sizeof(RenderCommandFn);

            uint32_t size = *(uint32_t*)buffer;
            buffer += sizeof(uint32_t);
            function(buffer);
            buffer += size;
        }

        commandBufferPtr_ = commandBuffer_;
        commandCount_ = 0;
    }
} // namespace CG

#pragma once

#include <functional>

namespace CG {

    class RenderCommandQueue {
    public:
        using RenderCommandFn = void (*)(void*);

        RenderCommandQueue();

        ~RenderCommandQueue();

        void* Allocate(RenderCommandFn func, uint32_t size);

        void Execute();

    private:
        uint8_t* commandBuffer_;
        uint8_t* commandBufferPtr_;
        uint32_t commandCount_ {};
    };

} // namespace CG

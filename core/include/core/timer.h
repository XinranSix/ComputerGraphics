#pragma once

#include <chrono>
#include <unordered_map>

#include "core/base/base.h"
#include "core/log/log.h"

namespace CG {

    class Timer {
    public:
        CG_FORCE_INLINE Timer() { Reset(); }

        CG_FORCE_INLINE void Reset() { start_ = std::chrono::high_resolution_clock::now(); }

        CG_FORCE_INLINE float Elapsed() {
            return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() -
                                                                         start_)
                       .count() *
                   0.001f * 0.001f;
        }

        CG_FORCE_INLINE float ElapsedMillis() {
            return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() -
                                                                         start_)
                       .count() *
                   0.001f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    };

} // namespace CG

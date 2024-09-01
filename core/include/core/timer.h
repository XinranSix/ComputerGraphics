#pragma once

#include <chrono>

namespace CG {
    class Timer {
    public:
        Timer() { Reset(); }

        void Reset() { strat_ = std::chrono::high_resolution_clock::now(); }

        float Elapsed() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() -
                                                                        strat_)
                       .count() *
                   0.001f * 0.001f * 0.001f;
        }

        float ElapsedMillis() { return Elapsed() * 1000.0f; }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> strat_;
    };
} // namespace CG

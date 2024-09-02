#pragma once

#include "core/base/assert.h"

/* #define CG_CORE_DEBUG // TODO: Remove this line when the project is ready for release

#ifdef CG_CORE_DEBUG
    #define CG_CORE_ENABLE_ASSERTS
#endif

// TODO: __debugbreak is a Windows specific function. Implement a cross-platform solution.
#ifdef CG_CORE_ENABLE_ASSERTS
    #define CG_CORE_ASSERT(x, ...)                                                                                     \
        {                                                                                                              \
            if (!(x)) {                                                                                                \
                CG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                   \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
#else
    #define CG_CORE_ASSERT(x, ...)
#endif */

// __VA_ARGS__ expansion to get past MSVC "bug"
#define CG_EXPAND_VARGS(x) x

// Detect compiler and define macros accordingly
#if defined(_MSC_VER)
    // Microsoft Visual Studio
    #define CG_FORCE_INLINE __forceinline
    #define CG_EXPLICIT_STATIC static
#elif defined(__GNUC__) || defined(__clang__)
// GCC or Clang
    #define CG_FORCE_INLINE __attribute__((always_inline)) inline
    #define CG_EXPLICIT_STATIC
#else
// Other compilers
    #define CG_FORCE_INLINE inline
    #define CG_EXPLICIT_STATIC
#endif

#define BIT(x) (1 << x)

#define CG_CORE_BIND_EVENT_FN(fn)                                                                                      \
    [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace CG {
    template <typename T>
    using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace CG

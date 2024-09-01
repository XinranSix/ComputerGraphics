#pragma once

#include "core/log.h"

#define CG_CORE_DEBUG // TODO: Remove this line when the project is ready for release

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
#endif

#define BIT(x) (1 << x)

#define CG_CORE_BIND_EVENT_FN(fn)                                                                                      \
    [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

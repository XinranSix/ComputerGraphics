#pragma once

#include "core/log/log.h"

#define CG_DEBUG

#ifdef CG_DEBUG
    #define CG_ENABLE_ASSERTS
#endif

#if defined(_MSC_VER) // Windows
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__) // GCC or Clang on Linux, macOS
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #define DEBUG_BREAK()                                                                                              \
        do {                                                                                                           \
        } while (0) // 未知平台，空操作
#endif

// clang-format off
#ifdef CG_ENABLE_ASSERTS
	#define CG_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { CG_APP_ERROR("Assertion Failed"); DEBUG_BREAK(); } }
	#define CG_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { CG_APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }

	#define CG_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
	#define CG_GET_ASSERT_MACRO(...) CG_EXPAND_VARGS(CG_ASSERT_RESOLVE(__VA_ARGS__, CG_ASSERT_MESSAGE, CG_ASSERT_NO_MESSAGE))

	#define CG_ASSERT(...) CG_EXPAND_VARGS( CG_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
	#define CG_CORE_ASSERT(...) CG_EXPAND_VARGS( CG_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
	#define CG_ASSERT(...)
	#define CG_CORE_ASSERT(...)
#endif
// clang-format on

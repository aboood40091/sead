#ifndef SEAD_TYPES_H_
#define SEAD_TYPES_H_

#ifdef cafe

#include <cstddef>
#include <types.h>

typedef u32 uintptr;
typedef s32 sintptr;
typedef s64 d_sintptr;

typedef uintptr uintptr_t;
typedef sintptr  intptr_t;

#ifndef VSCODE_INTELLISENSE

#ifndef override
    #define override
#endif // override

#endif // VSCODE_INTELLISENSE

#ifndef forceinline
    #define forceinline __attribute__((always_inline))
#endif // forceinline

#ifndef nullptr
    #define nullptr NULL
#endif // nullptr

#ifndef static_assert
    // https://stackoverflow.com/a/1597129
    #define TOKENPASTE(x, y) x ## y
    #define TOKENPASTE2(x, y) TOKENPASTE(x, y)

    #define static_assert(condition, ...) typedef int TOKENPASTE2(static_assert_, __LINE__)[(condition) ? 1 : -1]
#endif // static_assert

#else

#error "Unknown platform"

#endif // cafe

#ifndef VSCODE_INTELLISENSE

#ifdef __ghs__
    #define noinline  __noinline
#else
    #define noinline
#endif // __ghs__

#else

#define noinline

#endif

// TODO: Move to the proper headers
#define SEAD_MACRO_UTIL_ROUNDUP(x, y) ((x) + ((y) - 1) & ~((y) - 1))
#define SEAD_MACRO_UTIL_ROUNDDOWN(x, y) ((x) & ~((y) - 1))

#endif // SEAD_TYPES_H_

#ifndef SEAD_TYPES_H_
#define SEAD_TYPES_H_

#ifdef cafe

#include <types.h>

typedef u32 uintptr_t;
typedef s32  intptr_t;

// https://stackoverflow.com/a/1597129
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define static_assert(condition, ...) typedef int TOKENPASTE2(static_assert_, __LINE__)[(condition) ? 1 : -1]

#else
#error "Unknown platform"
#endif // cafe

#endif // SEAD_NEW_H_

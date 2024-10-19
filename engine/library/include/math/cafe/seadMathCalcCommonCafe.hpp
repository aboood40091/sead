#pragma once

#include <ppc_ghs.h>

namespace sead {

template <>
inline f32 MathCafe<f32>::rsqrt(f32 register x)
{
    //SEAD_ASSERT_MSG(x >= 0.0f, "x[%f] is out of the domain.", x);

    if (x <= 0.0f)
        return 0.0f;

    f32 register v0;
    f32 register v1;
    f32 register v2;
    const f32 register HALF  = 0.5f;
    const f32 register THREE = 3.0f;

#ifdef SEAD_MATH_CAFE_RSQRT_USE_INTRINSICS
    // The use of inline assembly in an inline function can be very dangerous,
    // and sometimes triggers compiler bugs in GHS.
    // Therefore, use intrinsics instead.
    // (For use with custom applications)
    v0 = __FRSQRTE(x);
    v1 = v0 * v0;
    v2 = v0 * HALF;
    v1 = __FNMSUBS(v1, x, THREE);
    v0 = v1 * v2;
#else
    asm ("frsqrte %[v0], %[x]                 \n\t"
         "fmuls   %[v1], %[v0], %[v0]         \n\t"
         "fmuls   %[v2], %[v0], %[HALF]       \n\t"
         "fnmsubs %[v1], %[v1], %[x], %[THREE]\n\t"
         "fmuls   %[v0], %[v1], %[v2]             "
         : [v0] "+f"(v0), [  v1] "+f"(  v1), [   v2] "+f"(   v2)
         : [ x]  "f"( x), [HALF]  "f"(HALF), [THREE]  "f"(THREE));
#endif

    return v0;
}

} // namespace sead

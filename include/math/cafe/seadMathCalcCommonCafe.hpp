#pragma once

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

    asm ("frsqrte %[v0], %[x]                 \n\t"
         "fmuls   %[v1], %[v0], %[v0]         \n\t"
         "fmuls   %[v2], %[v0], %[HALF]       \n\t"
         "fnmsubs %[v1], %[v1], %[x], %[THREE]\n\t"
         "fmuls   %[v0], %[v1], %[v2]             "
         : [v0] "+f"(v0), [  v1] "+f"(  v1), [   v2] "+f"(   v2)
         : [ x]  "f"( x), [HALF]  "f"(HALF), [THREE]  "f"(THREE));

    return v0;
}

} // namespace sead

#ifndef SEAD_COLOR_H_
#define SEAD_COLOR_H_

#include <basis/seadTypes.h>

namespace sead {

struct Color4f
{
    Color4f()
        : r(cElementMin)
        , g(cElementMin)
        , b(cElementMin)
        , a(cElementMax)
    {
    }

    Color4f(f32 _r, f32 _g, f32 _b, f32 _a)
        : r(_r)
        , g(_g)
        , b(_b)
        , a(_a)
    {
    }

    void setf(f32 _r, f32 _g, f32 _b, f32 _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    union
    {
        struct
        {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };
        f32 c[4];
    };

    static const f32 cElementMin = 0.0f;
    static const f32 cElementMax = 1.0f;

    static const Color4f cWhite;
    static const Color4f cRed;
    static const Color4f cGreen;
    static const Color4f cBlue;
};
#ifdef cafe
static_assert(sizeof(Color4f) == 0x10, "sead::Color4f size mismatch");
#endif // cafe

}  // namespace sead

#endif // #define SEAD_COLOR_H_

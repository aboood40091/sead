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

    ~Color4f()
    {
    }

    void setf(f32 _r, f32 _g, f32 _b, f32 _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    void setLerp(const Color4f& from, const Color4f& to, f32 ratio);

    Color4f& operator+=(const Color4f& rhs);
    Color4f& operator*=(const Color4f& rhs);
    Color4f& operator*=(f32 rhs);

    friend Color4f operator*(const Color4f& lhs, const Color4f& rhs);
    friend Color4f operator*(const Color4f& lhs, f32 rhs);
    friend bool operator==(const Color4f& lhs, const Color4f& rhs);

    union
    {
        struct
        {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };
        struct
        {
            f32 c[4];
        };
    };

    static const f32 cElementMax;
    static const f32 cElementMin;

    static const Color4f cBlack;
    static const Color4f cGray;
    static const Color4f cWhite;
    static const Color4f cRed;
    static const Color4f cGreen;
    static const Color4f cBlue;
    static const Color4f cYellow;
    static const Color4f cMagenta;
    static const Color4f cCyan;
};
#ifdef cafe
static_assert(sizeof(Color4f) == 0x10, "sead::Color4f size mismatch");
#endif // cafe

struct Color4u8
{
    Color4u8()
        : r(cElementMin)
        , g(cElementMin)
        , b(cElementMin)
        , a(cElementMax)
    {
    }

    Color4u8(u8 _r, u8 _g, u8 _b, u8 _a)
        : r(_r)
        , g(_g)
        , b(_b)
        , a(_a)
    {
    }

    ~Color4u8()
    {
    }

    void setf(f32 _r, f32 _g, f32 _b, f32 _a);

    void setLerp(const Color4u8& from, const Color4u8& to, f32 ratio);

    static Color4u8 lerp(const Color4u8& from, const Color4u8& to, f32 ratio);

    union
    {
        struct
        {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
        u32 rgba;
    };

    static const u8 cElementMax = 255;
    static const u8 cElementMin = 0;

    static const Color4u8 cBlack;
    static const Color4u8 cGray;
    static const Color4u8 cWhite;
    static const Color4u8 cRed;
    static const Color4u8 cGreen;
    static const Color4u8 cBlue;
    static const Color4u8 cYellow;
    static const Color4u8 cMagenta;
    static const Color4u8 cCyan;
};
#ifdef cafe
static_assert(sizeof(Color4u8) == 4, "sead::Color4u8 size mismatch");
#endif // cafe

}  // namespace sead

#endif // #define SEAD_COLOR_H_

#include <gfx/seadColor.h>
#include <math/seadMathCalcCommon.h>

namespace sead {

const Color4f Color4f::cBlack(0.0f, 0.0f, 0.0f, 1.0f);
const Color4f Color4f::cWhite(1.0f, 1.0f, 1.0f, 1.0f);
const Color4f Color4f::cRed  (1.0f, 0.0f, 0.0f, 1.0f);
const Color4f Color4f::cGreen(0.0f, 1.0f, 0.0f, 1.0f);
const Color4f Color4f::cBlue (0.0f, 0.0f, 1.0f, 1.0f);

const f32 Color4f::cElementMax = 1.0f;
const f32 Color4f::cElementMin = 0.0f;

void Color4f::setLerp(const Color4f& from, const Color4f& to, f32 ratio)
{
    ratio = Mathf::clamp2(0.0f, ratio, 1.0f); // I find it funny that they didn't use cElementMin/cElementMax
    a = Mathf::lerp(from.a, to.a, ratio);
    r = Mathf::lerp(from.r, to.r, ratio);
    g = Mathf::lerp(from.g, to.g, ratio);
    b = Mathf::lerp(from.b, to.b, ratio);
}

Color4f& Color4f::operator+=(const Color4f& rhs)
{
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    a += rhs.a;
    return *this;
}

Color4f& Color4f::operator*=(const Color4f& rhs)
{
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    a *= rhs.a;
    return *this;
}

Color4f operator*(const Color4f& lhs, const Color4f& rhs)
{
    Color4f tmp = lhs;
    tmp *= rhs;
    return tmp;
}

bool operator==(const Color4f& lhs, const Color4f& rhs)
{
    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b &&
           lhs.a == rhs.a;
}

} // namespace sead

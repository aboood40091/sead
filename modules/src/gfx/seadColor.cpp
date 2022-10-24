#include <gfx/seadColor.h>

namespace sead {

const Color4f Color4f::cBlack(0.0f, 0.0f, 0.0f, 1.0f);
const Color4f Color4f::cWhite(1.0f, 1.0f, 1.0f, 1.0f);
const Color4f Color4f::cRed  (1.0f, 0.0f, 0.0f, 1.0f);
const Color4f Color4f::cGreen(0.0f, 1.0f, 0.0f, 1.0f);
const Color4f Color4f::cBlue (0.0f, 0.0f, 1.0f, 1.0f);

const f32 Color4f::cElementMax = 1.0f;
const f32 Color4f::cElementMin = 0.0f;

bool operator==(const Color4f& lhs, const Color4f& rhs)
{
    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b &&
           lhs.a == rhs.a;
}

} // namespace sead

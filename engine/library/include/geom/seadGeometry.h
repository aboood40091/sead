#ifndef SEAD_GEOMETRY_H_
#define SEAD_GEOMETRY_H_

#include <geom/seadLine.h>
#include <geom/seadPlane.h>

namespace sead { namespace Geometry {

enum IntersectionResult
{
    cIntersectionResult_None    = 0,
    cIntersectionResult_One     = 1,
    cIntersectionResult_Two     = 2,
    cIntersectionResult_OnPlane = 3
};

IntersectionResult calcIntersectionSegmentToPlane(const Segment3f& segment, const Plane3f& plane, f32* p_t = nullptr);

} } // namespace sead::Geometry

#endif // SEAD_GEOMETRY_H_

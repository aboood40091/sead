#pragma once

#include <math/seadBoundBox.h>
#include <math/seadSphere.h>
#include <prim/seadBitFlag.h>

namespace agl { namespace sdw {

class DepthShadow
{
    typedef sead::BitFlag32 (DepthShadow::*CheckAndUpdateBoundBox)(const sead::BoundBox3f&);
    typedef sead::BitFlag32 (DepthShadow::*CheckAndUpdateSphere)(const sead::Sphere3f&);

public:
    DepthShadow();
    virtual ~DepthShadow();

    sead::BitFlag32 checkAndUpdate(const sead::BoundBox3f& box)
    {
        return (this->*mpCheckAndUpdateBoundBox)(box);
    }

    sead::BitFlag32 checkAndUpdate(const sead::Sphere3f& sphere)
    {
        return (this->*mpCheckAndUpdateSphere)(sphere);
    }

private:
    CheckAndUpdateBoundBox mpCheckAndUpdateBoundBox;
    CheckAndUpdateSphere mpCheckAndUpdateSphere;
    u32 _10[(0xC04 - 0x10) / sizeof(u32)];
};
static_assert(sizeof(DepthShadow) == 0xC08, "agl::sdw::DepthShadow size mismatch");

} }

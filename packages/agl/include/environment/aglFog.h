#pragma once

#include <environment/aglEnvObj.h>

namespace agl { namespace env {

class EnvObjBuffer;

class Fog : public EnvObj
{
    SEAD_RTTI_OVERRIDE(Fog, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()

public:
    Fog();
    virtual ~Fog();

public:
    f32 getStart() const { return *mStart; }
    f32 getEnd() const { return *mEnd; }
    f32 getStartEndInv() const { return *mEnd == *mStart ? 1.0f : (1.0f / (*mEnd - *mStart)); }
    const sead::Color4f& getColor() const { return *mColor; }
    const sead::Vector3f& getDirection() const { return *mDirection; }

private:
    utl::Parameter<f32> mStart;
    utl::Parameter<f32> mEnd;
    utl::Parameter<sead::Color4f> mColor;
    agl::utl::Parameter<sead::Vector3f> mDirection;
};
static_assert(sizeof(Fog) == 0x16C);

} }

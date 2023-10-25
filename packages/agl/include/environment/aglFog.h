#pragma once

#include <environment/aglEnvObj.h>

namespace agl { namespace env {

class EnvObjBuffer;

class Fog : public EnvObj
{
public:
    static s32* sTypeInfo;

public:
    Fog();
    virtual ~Fog();

    virtual s32 getTypeID() const { return *sTypeInfo; }

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

#pragma once

#include <environment/aglEnvObj.h>
#include <utility/aglParameterDirection.h>

namespace agl { namespace env {

class EnvObjBuffer;

class Fog : public EnvObj
{
    SEAD_RTTI_OVERRIDE(Fog, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()

public:
    Fog();
    virtual ~Fog() { }

    virtual ShaderMode drawDebug(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index, ShaderMode mode) const;

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
    utl::ParameterDirection3f mDirection;
};
static_assert(sizeof(Fog) == 0x16C);

} }

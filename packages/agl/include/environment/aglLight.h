#pragma once

#include <environment/aglEnvObj.h>

namespace agl { namespace env {

class AmbientLight : public EnvObj
{
    SEAD_RTTI_OVERRIDE(AmbientLight, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()

public:
    AmbientLight();
    virtual ~AmbientLight() { }

    virtual void update();
    virtual ShaderMode drawDebug(const sead::Matrix34f&, const sead::Matrix44f&, s32, ShaderMode mode) const;

    sead::Color4f getColor() const
    {
        f32 intensity = *mIntensity;
        sead::Color4f color = *mColor * sead::Color4f(intensity, intensity, intensity, intensity);
        return color;
    }

private:
    utl::Parameter<sead::Color4f> mColor;
    utl::Parameter<f32> mIntensity;
};
static_assert(sizeof(AmbientLight) == 0x13C);

class HemisphereLight : public EnvObj
{
    SEAD_RTTI_OVERRIDE(HemisphereLight, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()
    // TODO
};

class DirectionalLight : public EnvObj
{
    SEAD_RTTI_OVERRIDE(DirectionalLight, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()
    // TODO
};

} }

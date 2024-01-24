#pragma once

#include <container/seadBuffer.h>
#include <environment/aglEnvObj.h>
#include <math/seadMatrix.h>
#include <utility/aglParameterDirection.h>

namespace agl { namespace env {

class AmbientLight : public EnvObj
{
    SEAD_RTTI_OVERRIDE(AmbientLight, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()

public:
    AmbientLight();
    virtual ~AmbientLight();

    virtual void update();
    virtual ShaderMode drawDebug(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index, ShaderMode mode) const;

public:
    const sead::Color4f& getColor() const { return *mColor; }
    f32 getIntensity() const { return *mIntensity; }

private:
    utl::Parameter<sead::Color4f> mColor;
    utl::Parameter<f32> mIntensity;
};
static_assert(sizeof(AmbientLight) == 0x13C);

class HemisphereLight : public EnvObj
{
    SEAD_RTTI_OVERRIDE(HemisphereLight, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()

public:
    HemisphereLight();
    virtual ~HemisphereLight();

    virtual void initialize(s32 view_max, sead::Heap* heap);
    virtual void updateView(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index);
    virtual ShaderMode drawDebug(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index, ShaderMode mode) const;

public:
    const sead::Color4f& getSkyColor() const { return *mSkyColor; }
    const sead::Color4f& getGroundColor() const { return *mGroundColor; }
    f32 getIntensity() const { return *mIntensity; }
    const sead::Vector3f& getDirection() const { return *mDirection; }
    const sead::Vector3f& getDiffuseDir(s32 view_index) const { return mDiffuseDir[view_index]; }

private:
    utl::Parameter<sead::Color4f> mSkyColor;
    utl::Parameter<sead::Color4f> mGroundColor;
    utl::Parameter<f32> mIntensity;
    utl::ParameterDirection3f mDirection;
    sead::Buffer<sead::Vector3f> mDiffuseDir;
};
static_assert(sizeof(HemisphereLight) == 0x180);

class DirectionalLight : public EnvObj
{
    SEAD_RTTI_OVERRIDE(DirectionalLight, EnvObj)
    AGL_ENV_OBJ_TYPE_INFO()

public:
    DirectionalLight();
    virtual ~DirectionalLight();

    virtual void initialize(s32 view_max, sead::Heap* heap);
    virtual void updateView(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index);
    virtual ShaderMode drawDebug(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index, ShaderMode mode) const;
    virtual void callbackLoadData();

public:
    const sead::Color4f& getDiffuseColor() const { return *mDiffuseColor; }
    const sead::Color4f& getSpecularColor() const { return *mSpecularColor; }
    const sead::Color4f& getBacksideColor() const { return *mBacksideColor; }
    f32 getIntensity() const { return *mIntensity; }
    const sead::Vector3f& getDirection() const { return *mDirection; }
    const sead::Vector3f& getDiffuseDir(s32 view_index) const { return mDiffuseDir[view_index]; }

private:
    void syncFromDirection_();

private:
    utl::Parameter<sead::Color4f> mDiffuseColor;
    utl::Parameter<sead::Color4f> mSpecularColor;
    utl::Parameter<sead::Color4f> mBacksideColor;
    utl::Parameter<f32> mIntensity;
    utl::ParameterDirection3f mDirection;
    sead::Buffer<sead::Vector3f> mDiffuseDir;
    sead::Matrix34f _1a0;
    sead::Vector3f _1d0;
    sead::Vector2f _1dc;
};
static_assert(sizeof(DirectionalLight) == 0x1E4);

} }

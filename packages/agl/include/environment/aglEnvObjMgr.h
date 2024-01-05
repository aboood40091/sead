#pragma once

#include <environment/aglEnvObjSet.h>
#include <math/seadMatrix.h>
#include <utility/aglNamedObjMgr.h>
#include <utility/aglParameterIO.h>
#include <utility/aglResParameter.h>

namespace agl { namespace env {

class EnvObjMgr : public EnvObjBuffer, public utl::INamedObjMgr, public utl::IParameterIO
{
public:
    class InitArg : public AllocateArg
    {
    public:
        InitArg();

        void setGroupMax(s32 max)
        {
            mGroupMax = max;
        }

        s32 getGroupMax() const
        {
            return mGroupMax;
        }

        void setViewMax(s32 max)
        {
            mViewMax = max;
        }

        s32 getViewMax() const
        {
            return mViewMax;
        }

    private:
        s32 mGroupMax;
        s32 mViewMax;
    };
    static_assert(sizeof(InitArg) == 0x110);

public:
    EnvObjMgr();
    virtual ~EnvObjMgr();

    // TODO: Virtual functions

    void initialize(const InitArg& arg, sead::Heap* heap = nullptr);

    void applyResource(utl::ResParameterArchive arc)
    {
        applyResource_(arc, arc, 1.0f);
    }

    void applyResourceLerp(utl::ResParameterArchive arc_a, utl::ResParameterArchive arc_b, f32 t)
    {
        applyResource_(arc_a, arc_b, t);
    }

    void update();

    void updateView(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index);

    const EnvObjSet& getEnvObjSet() const
    {
        return mEnvObjSet;
    }

private:
    void applyResource_(utl::ResParameterArchive arc_a, utl::ResParameterArchive arc_b, f32 t);

private:
    u32 _234[(0x250- 0x234) / sizeof(u32)];
    EnvObjSet mEnvObjSet;
    u32 _42C[(0x464 - 0x42C) / sizeof(u32)];
};
static_assert(sizeof(EnvObjMgr) == 0x464, "agl::env::EnvObjMgr size mismatch");

} }

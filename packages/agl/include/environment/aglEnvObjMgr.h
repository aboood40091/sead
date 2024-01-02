#pragma once

#include <environment/aglEnvObjBuffer.h>
#include <math/seadMatrix.h>
#include <utility/aglResParameter.h>

namespace agl { namespace env {

// TODO
class EnvObjSet
{
public:
    u32 _0[0x1DC / sizeof(u32)];
};
static_assert(sizeof(EnvObjSet) == 0x1DC);

class EnvObjMgr : public EnvObjBuffer
{
public:
    EnvObjMgr();
    virtual ~EnvObjMgr();

    // TODO: Virtual functions

    void applyResource(utl::ResParameterArchive arc)
    {
        applyResource_(arc, arc, 1.0f);
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
    u32 _20[0x230 / sizeof(u32)];
    EnvObjSet mEnvObjSet;
    u32 _42C[(0x464 - 0x42C) / sizeof(u32)];
};
static_assert(sizeof(EnvObjMgr) == 0x464, "agl::env::EnvObjMgr size mismatch");

} }

#pragma once

#include <utility/aglResParameter.h>

namespace agl { namespace env {

class EnvObjMgr
{
public:
    EnvObjMgr();

    void applyResource(utl::ResParameterArchive arc)
    {
        applyResource_(arc, arc, 1.0f);
    }

private:
    void applyResource_(utl::ResParameterArchive arc_a, utl::ResParameterArchive arc_b, f32 t);

private:
    u32 _0[0x464 / sizeof(u32)];
};
static_assert(sizeof(EnvObjMgr) == 0x464, "agl::env::EnvObjMgr size mismatch");

} }

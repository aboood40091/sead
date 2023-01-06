#pragma once

#include <container/seadBuffer.h>
#include <lighting/aglLightMap.h>

namespace agl { namespace lght {

class LightMapMgr /* : public utl::IParameterIO */
{
public:
    LightMapMgr();
    /* virtual */ ~LightMapMgr();

    LightMap& getLightMap(s32 index) { return mLightMap[index]; }
    const LightMap& getLightMap(s32 index) const { return mLightMap[index]; }

    s32 searchIndex(const sead::SafeString& name) const;

private:
    u32 _0[(0x1E0 - 0x0) / sizeof(u32)];
    sead::Buffer<LightMap> mLightMap;
    u32 _1e8[(0x2D6C - 0x1E8) / sizeof(u32)];
};
static_assert(sizeof(LightMapMgr) == 0x2D6C, "agl::lght::LightMapMgr size mismatch");

} }

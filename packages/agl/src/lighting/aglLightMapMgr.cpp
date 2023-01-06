#include <lighting/aglLightMapMgr.h>

namespace agl { namespace lght {

s32 LightMapMgr::searchIndex(const sead::SafeString& name) const
{
    for (sead::Buffer<LightMap>::constIterator it = mLightMap.begin(), it_end = mLightMap.end(); it != it_end; ++it)
        if (name.isEqual(it->mName))
            return it.getIndex();

    return -1;
}

} }

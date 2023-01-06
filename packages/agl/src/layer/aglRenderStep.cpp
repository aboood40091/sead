#include <layer/aglRenderStep.h>

namespace agl { namespace lyr {

s32 RenderStep::remove(const DrawMethod* p_method)
{
    // SEAD_ASSERT(p_method != nullptr);
    sead::FixedPtrArray<DrawMethod, 256>::iterator itr = mpDrawMethod.begin();
    s32 ret = 0;
    s32 idx = 0;
    while (itr != mpDrawMethod.end())
    {
        if (&(*itr) == p_method)
        {
            mpDrawMethod.erase(idx);
            ++ret;
        }
        else
        {
            ++itr;
            ++idx;
        }
    }
    return ret;
}

} }

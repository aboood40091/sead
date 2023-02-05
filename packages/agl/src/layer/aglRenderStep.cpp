#include <layer/aglRenderStep.h>

namespace agl { namespace lyr {

bool RenderStep::pushBack(DrawMethod* p_method)
{
    // SEAD_ASSERT(p_method != nullptr);
    for (sead::FixedPtrArray<DrawMethod, 256>::iterator itr = mpDrawMethod.begin(); itr != mpDrawMethod.end(); ++itr)
        if (&(*itr) == p_method)
            return false;

    mpDrawMethod.pushBack(p_method);
    return true;
}

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

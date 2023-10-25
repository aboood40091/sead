#include <layer/aglLayer.h>
#include <layer/aglRenderStep.h>

namespace agl { namespace lyr {

DrawMethod* Layer::pushBackDrawMethod(u32 render_step, DrawMethod* p_method)
{
    // SEAD_ASSERT(p_method);
    if (!mRenderStep[render_step].pushBack(p_method))
        return nullptr;

    return p_method;
}

DrawMethod* Layer::pushBackDrawMethod(DrawMethod* p_method)
{
    // SEAD_ASSERT(p_method);
    bool success = false;
    for (sead::Buffer<RenderStep>::iterator itr_render_step = mRenderStep.begin(), itr_render_step_end = mRenderStep.end(); itr_render_step != itr_render_step_end; ++itr_render_step)
        success |= pushBackDrawMethod(itr_render_step.getIndex(), p_method) != nullptr;
    return success ? p_method : nullptr;
}

s32 Layer::removeDrawMethod(const DrawMethod* p_draw_method)
{
    // SEAD_ASSERT(p_draw_method != nullptr);
    s32 ret = 0;
    for (sead::Buffer<RenderStep>::iterator itr = mRenderStep.begin(), itr_end = mRenderStep.end(); itr != itr_end; ++itr)
        ret += (*itr).remove(p_draw_method);
    return ret;
}

void Layer::removeDrawMethod(u32 render_step, const DrawMethod* p_draw_method)
{
    // SEAD_ASSERT(p_draw_method != nullptr);
    mRenderStep[render_step].remove(p_draw_method);
}

} }

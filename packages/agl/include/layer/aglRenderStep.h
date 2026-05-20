#pragma once

#include <container/seadPtrArray.h>
#include <prim/seadBitFlag.h>

namespace agl { namespace lyr {

class DrawMethod;
class RenderInfo;

class RenderStep
{
public:
    RenderStep();
    virtual ~RenderStep() {}

    void draw(const RenderInfo& render_info) const;

    bool pushBack(DrawMethod* p_method);
    s32 remove(const DrawMethod* p_method);

    void setEnable(bool enable)
    {
        mFlag.changeBit(0, enable);
    }

    bool isEnable() const
    {
        return mFlag.isOnBit(0);
    }

protected:
    sead::FixedPtrArray<DrawMethod, 256> mpDrawMethod;
    sead::BitFlag32 mFlag; // & 1: render
};
static_assert(sizeof(RenderStep) == 0x414, "agl::lyr::RenderStep size mismatch");

} }

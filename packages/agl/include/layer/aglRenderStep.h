#pragma once

#include <container/seadPtrArray.h>
#include <prim/seadBitFlag.h>

namespace agl { namespace lyr {

class DrawMethod;

class RenderStep
{
public:
    RenderStep();
    virtual ~RenderStep() {}

    bool pushBack(DrawMethod* p_method);
    s32 remove(const DrawMethod* p_method);

private:
    sead::FixedPtrArray<DrawMethod, 256> mpDrawMethod;
    sead::BitFlag32 mFlag; // & 1: render
};
static_assert(sizeof(RenderStep) == 0x414, "agl::lyr::RenderStep size mismatch");

} }

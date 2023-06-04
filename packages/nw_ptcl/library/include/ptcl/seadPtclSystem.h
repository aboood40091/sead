#ifndef SEAD_PTCL_SYSTEM_H_
#define SEAD_PTCL_SYSTEM_H_

#include <ptcl/seadPtclData.h>
#include <ptcl/seadCallback.h>

#include <ptcl/seadHeap.h>
#include <ptcl/seadPtclEditorInterface.h>
#include <time/seadTickTime.h>

#include <nw/effect.h>

namespace nw { namespace eftvw { // TODO: Implement this properly

class ViewerSystem
{
public:
    void UnbindResource(u32 resourceId);
};

} }

namespace sead { namespace ptcl {

class PtclSystem : public nw::eft::System
{
public:
    void clearResource(s32 resId)
    {
        if (mpViewerSys != nullptr)
            mpViewerSys->UnbindResource(resId);

        Heap heap(mSeadHeapArray[resId]);
        // SOME_MYSTERIOUS_ASSERT_FUNCTION(mSeadHeapArray[resId]);

        ClearResource(&heap, resId);

        mSeadHeapArray[resId] = nullptr;
    }

private:
    TickTime mTime;
    Heap mHeap;
    Heap mViewerSysHeap;
    Heap _a40;
    u8 _a48[4];
    ::sead::Heap** mSeadHeapArray;
    nw::eftvw::ViewerSystem* mpViewerSys;
    PtclEditorInterface mEditorInterface;
    u8 _18ac[4];
};
static_assert(sizeof(PtclSystem) == 0x18B0, "sead::ptcl::PtclSystem size mismatch");

} }

#endif // SEAD_PTCL_SYSTEM_H_

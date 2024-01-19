#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace {

const u32 cDestructedFlag = 1;

} // namespace {no-name}

namespace sead {

IDisposer::IDisposer()
    : mListNode()
    , mDisposerHeap(nullptr)
{
    HeapMgr* heap_mgr = HeapMgr::instance();
    if (heap_mgr != nullptr)
    {
        mDisposerHeap = heap_mgr->findContainHeap(this);
        if (mDisposerHeap != nullptr)
            mDisposerHeap->appendDisposer_(this);
    }
}

IDisposer::~IDisposer()
{
    if (*(uintptr_t*)&mDisposerHeap == cDestructedFlag)
    {
        //SEAD_ASSERT_MSG(false, "Destruct twice. [%p] Your class has possibilities for wrong order of multipleinheritance.", this);
    }
    else
    {
        if (mDisposerHeap != nullptr)
            mDisposerHeap->removeDisposer_(this);

        *(uintptr_t*)&mDisposerHeap = cDestructedFlag;
    }
}

} // namespace sead

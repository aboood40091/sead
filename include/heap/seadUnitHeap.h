#ifndef SEAD_UNIT_HEAP_H_
#define SEAD_UNIT_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

// TODO
class FreeList
{
public:
    void* mFree;
    void* mWork;
};

class UnitHeap : public Heap
{
    SEAD_RTTI_OVERRIDE(UnitHeap, Heap)

public:
    UnitHeap(const SafeString& name, Heap* parent, void* address, u32 size, u32, bool);
    virtual ~UnitHeap();

    virtual void destroy();
    virtual u32 adjust();
    virtual void* tryAlloc(size_t size, s32 alignment);
    virtual void free(void* ptr);
    virtual void* resizeFront(void*, size_t);
    virtual void* resizeBack(void*, size_t);
    virtual void freeAll();
    virtual u32 getStartAddress() const;
    virtual u32 getEndAddress() const;
    virtual u32 getSize() const;
    virtual u32 getFreeSize() const;
    virtual u32 getMaxAllocatableSize(int) const;
    virtual bool isInclude(const void*) const;
    virtual bool isFreeable() const;
    virtual bool isResizable() const;
    virtual bool isAdjustable() const;

protected:
    u32 mBlockSize;
    void* mAreaStart;
    u32 mAreaSize;
    u32 mFreeSize;
    FreeList mFreeList;
};

} // namespace sead

#endif // SEAD_UNIT_HEAP_H_

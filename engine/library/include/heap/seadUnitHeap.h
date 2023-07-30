#ifndef SEAD_UNIT_HEAP_H_
#define SEAD_UNIT_HEAP_H_

#include <container/seadFreeList.h>
#include <heap/seadHeap.h>

namespace sead {

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

public:
    static UnitHeap* tryCreate(u32 size, const SafeString& name, u32 block_size, s32 alignment = 4, Heap* parent = nullptr, bool enable_lock = false);
    static UnitHeap* tryCreateWithBlockNum(u32 block_size, u32 num, const SafeString& name, s32 alignment = 4, Heap* parent = nullptr, bool enable_lock = false);

protected:
    u32 mBlockSize;
    void* mAreaStart;
    u32 mAreaSize;
    u32 mFreeSize;
    FreeList mFreeList;
};
#ifdef cafe
static_assert(sizeof(UnitHeap) == 0xA8, "sead::UnitHeap size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_UNIT_HEAP_H_

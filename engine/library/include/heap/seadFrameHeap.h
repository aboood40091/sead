#ifndef SEAD_FRAME_HEAP_H_
#define SEAD_FRAME_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

class FrameHeap : public Heap
{
    SEAD_RTTI_OVERRIDE(FrameHeap, Heap)

public:
    FrameHeap(const SafeString& name, Heap* parent, void* start, u32 size, HeapDirection direction, bool enable_lock);
    virtual ~FrameHeap();

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
    virtual u32 getMaxAllocatableSize(s32 alignment = 4) const;
    virtual bool isInclude(const void*) const;
    virtual bool isFreeable() const;
    virtual bool isResizable() const;
    virtual bool isAdjustable() const;

public:
    static FrameHeap* tryCreate(u32 size, const SafeString& name, Heap* parent = nullptr, HeapDirection direction = cHeapDirection_Forward, bool enable_lock = false);

protected:
    void initialize_();

protected:
    class State
    {
        void* mHeadPtr;
        void* mTailPtr;

        friend class FrameHeap;
    };

    State state;
};
#ifdef cafe
static_assert(sizeof(FrameHeap) == 0x98, "sead::FrameHeap size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_FRAME_HEAP_H_

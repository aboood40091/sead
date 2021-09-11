#ifndef SEAD_HEAP_H_
#define SEAD_HEAP_H_

#include <stddef.h>

#include <container/seadOffsetList.h>
#include <heap/seadDisposer.h>
#include <prim/seadBitFlag.h>
#include <prim/seadNamable.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>
#include <thread/seadCriticalSection.h>

namespace sead { namespace hostio {

class Context;

} // namespace sead::hostio

class Thread;

class Heap : public IDisposer, public INamable
{
public:
    enum HeapDirection
    {
        cHeapDirection_Forward = 1,
        cHeapDirection_Reverse = -1
    };

private:
    typedef OffsetList<Heap> HeapList;
    typedef OffsetList<IDisposer> DisposerList;

public:
    Heap(const SafeString& name, Heap* parent, void* start, size_t size, HeapDirection direction, bool enable_lock);
    virtual ~Heap() { }

    SEAD_RTTI_BASE(Heap)

    virtual void destroy() = 0;
    virtual u32 adjust() = 0;

    void* alloc(size_t size, s32 alignment)
    {
        return tryAlloc(size, alignment);
    }

    virtual void* tryAlloc(size_t size, s32 alignment) = 0;
    virtual void free(void* ptr) = 0;
    virtual void* resizeFront(void* ptr, size_t new_size) = 0;
    virtual void* resizeBack(void* ptr, size_t new_size) = 0;
    virtual void freeAll() = 0;
    virtual u32 getStartAddress() const = 0;
    virtual u32 getEndAddress() const = 0;
    virtual u32 getSize() const = 0;
    virtual u32 getFreeSize() const = 0;
    virtual u32 getMaxAllocatableSize(s32 alignment) const = 0;
    Heap* getParent() const { return mParent; }
    virtual bool isInclude(const void* ptr) const = 0;
    HeapDirection getDirection() const { return mDirection; }
    virtual bool isFreeable() const = 0;
    virtual bool isResizable() const = 0;
    virtual bool isAdjustable() const = 0;
    void setEnableLock(bool enable_lock);
    bool isEnableLock() const;
    bool lock();
    bool unlock();
    void setEnableWarning(bool);
    bool isEnableWarning() const;
    void setAccessThread(Thread*);
    Thread* getAccessThread() const;

    virtual void dump() const
    {
    }

    virtual void genInformation_(hostio::Context* context);

protected:
    Heap* findContainHeap_(const void* ptr);
    bool hasNoChild_() const;
    //static void setEnableDebugFillSystem_(Heap*, bool);
    //static bool isEnableDebugFillSystem_(const Heap*);
    //bool isEnableDebugFillAlloc_() const;
    //bool isEnableDebugFillFree_() const;
    //bool isEnableDebugFillHeapDestroy_() const;
    void destruct_();
    void dispose_(const void*, const void*);
    void appendDisposer_(IDisposer* o);
    void removeDisposer_(IDisposer* o);
    void eraseChild_(Heap*);
    void checkAccessThread_() const;

public:
    HeapList::constIterator childBegin() const;
    HeapList::constIterator childEnd() const;
    DisposerList::constIterator disposerBegin() const;
    DisposerList::constIterator disposerEnd() const;

    void pushBackChild_(Heap*);

protected:
    void* mStart;
    size_t mSize;
    Heap* mParent;
    HeapList mChildren;
    ListNode mListNode;
    DisposerList mDisposerList;
    HeapDirection mDirection;
    CriticalSection mCS;
    BitFlag32 mFlag;

    friend IDisposer::IDisposer();
    friend IDisposer::~IDisposer();
    friend class HeapMgr;
};

} // namespace sead

#endif // SEAD_HEAP_H_

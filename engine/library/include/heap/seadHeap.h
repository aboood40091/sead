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

    class Flag
    {
    public:
        enum ValueType
        {
            cDontUseThis_StartNumMinus1 = -1,
            cEnableLock,
          //cDisposing,
          //cEnableWarning,
          //cEnableDebugFillSystem,
          //cEnableDebugFillUser,
          //cDontUseThis_MaxNumPlus1,
            cDontUseThis_MemSize32bit = 0x7FFFFFFF,
          //cEnumStart = cDontUseThis_StartNumMinus1 + 1,
          //cEnumMax = cDontUseThis_MaxNumPlus1 - 1
        };
    };

public:
    Heap(const SafeString& name, Heap* parent, void* start, size_t size, HeapDirection direction, bool enable_lock);
    virtual ~Heap() { }

    SEAD_RTTI_BASE(Heap)

    virtual void destroy() = 0;
    virtual size_t adjust() = 0;

    void* alloc(size_t size, s32 alignment)
    {
        return tryAlloc(size, alignment);
    }

    virtual void* tryAlloc(size_t size, s32 alignment) = 0;
    virtual void free(void* ptr) = 0;
    virtual void* resizeFront(void* ptr, size_t new_size) = 0;
    virtual void* resizeBack(void* ptr, size_t new_size) = 0;
    virtual void freeAll() = 0;
    virtual const void* getStartAddress() const = 0;
    virtual const void* getEndAddress() const = 0;
    virtual size_t getSize() const = 0;
    virtual size_t getFreeSize() const = 0;
    virtual size_t getMaxAllocatableSize(s32 alignment = 4) const = 0;
    Heap* getParent() const { return mParent; }
    virtual bool isInclude(const void* ptr) const = 0;
    HeapDirection getDirection() const { return mDirection; }
    virtual bool isFreeable() const = 0;
    virtual bool isResizable() const = 0;
    virtual bool isAdjustable() const = 0;

    void setEnableLock(bool enable_lock)
    {
        mFlag.changeBit(Flag::cEnableLock, enable_lock);
    }

    bool isEnableLock() const
    {
        return mFlag.isOnBit(Flag::cEnableLock);
    }

    bool lock();
    bool unlock();
    void setEnableWarning(bool);
    bool isEnableWarning() const;
    void setAccessThread(Thread*);
    Thread* getAccessThread() const;

    virtual void dump() const
    {
    }

protected:
    virtual void genInformation_(hostio::Context* context);

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
    OffsetList<Heap>::constIterator childBegin() const
    {
        return mChildren.constBegin();
    }

    OffsetList<Heap>::constIterator childEnd() const
    {
        return mChildren.constEnd();
    }

    u32 childSize() const
    {
        return mChildren.size();
    }

    OffsetList<IDisposer>::constIterator disposerBegin() const
    {
        return mDisposerList.constBegin();
    }

    OffsetList<IDisposer>::constIterator disposerEnd() const
    {
        return mDisposerList.constEnd();
    }

    u32 disposerSize() const
    {
        return mDisposerList.size();
    }

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
#ifdef cafe
static_assert(sizeof(Heap) == 0x90, "sead::Heap size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_HEAP_H_

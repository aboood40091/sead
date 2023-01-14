#ifndef SEAD_FREE_LIST_H_
#define SEAD_FREE_LIST_H_

#include <basis/seadTypes.h>

namespace sead {

class FreeList
{
public:
    FreeList()
        : mFree(nullptr)
        , mWork(nullptr)
    {
    }

    void init(void* work, s32 elem_size, s32 num)
    {
        // SEAD_ASSERT(work);
        // SEAD_ASSERT(elem_size > 0 && elem_size % 4 == 0);
        // SEAD_ASSERT(num > 0);

        const s32 idx_multiplier = elem_size / 4;

        mFree = work;

        for (s32 i = 0; i < num - 1; ++i)
            static_cast<void**>(work)[idx_multiplier * i] = &static_cast<void**>(work)[idx_multiplier * (i + 1)];

        static_cast<void**>(work)[(num - 1) * idx_multiplier] = nullptr;

        mWork = work;
    }

    void* get()
    {
        if (mFree == nullptr)
            return nullptr;

        void* ptr = mFree;
        mFree = *static_cast<void**>(mFree);
        return ptr;
    }

    void cleanup()
    {
        mFree = nullptr;
        mWork = nullptr;
    }

    void* work() const { return mWork; }

    void put(void* ptr)
    {
        *static_cast<void**>(ptr) = mFree;
        mFree = ptr;
    }

private:
    void* mFree;
    void* mWork;
};

} // namespace sead

#endif // SEAD_FREE_LIST_H_

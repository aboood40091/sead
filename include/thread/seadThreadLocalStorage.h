#ifndef SEAD_THREAD_LOCAL_STORAGE_H_
#define SEAD_THREAD_LOCAL_STORAGE_H_

#include <basis/seadTypes.h>

namespace sead {

class ThreadLocalStorage
{
public:
    ThreadLocalStorage()
        : mThreadLocalStorageInner(0)
    {
#ifdef cafe
        if (!sIsIndex0Used)
        {
            mThreadLocalStorageInner = 0;
            sIsIndex0Used = true;
        }
        else if (!sIsIndex1Used)
        {
            mThreadLocalStorageInner = 1;
            sIsIndex1Used = true;
        }
#endif // cafe
    }

#ifdef cafe
    s32 getValue() const
    {
        return mThreadLocalStorageInner;
    }
#endif // cafe

private:
#ifdef cafe
    static bool sIsIndex0Used;
    static bool sIsIndex1Used;
    s32 mThreadLocalStorageInner;
#endif // cafe
};
#ifdef cafe
static_assert(sizeof(ThreadLocalStorage) == 4, "sead::ThreadLocalStorage size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_THREAD_LOCAL_STORAGE_H_

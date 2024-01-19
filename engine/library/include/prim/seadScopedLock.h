#ifndef SEAD_SCOPED_LOCK_H_
#define SEAD_SCOPED_LOCK_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename T>
class ScopedLock
{
public:
    explicit ScopedLock(T* t)
        : mLocked(t)
    {
        mLocked->lock();
    }

    virtual ~ScopedLock()
    {
        mLocked->unlock();
    }

private:
    ScopedLock(const ScopedLock<T>&);
    const ScopedLock<T>& operator=(const ScopedLock<T>&);

protected:
    T* mLocked;
};

template <typename T>
class ConditionalScopedLock
{
public:
    explicit ConditionalScopedLock(T* t, bool cond)
        : mLocked(nullptr)
    {
        if (cond)
        {
            mLocked = t;
            mLocked->lock();
        }
    }

    virtual ~ConditionalScopedLock()
    {
        if (mLocked != nullptr)
            mLocked->unlock();
    }

private:
    ConditionalScopedLock(const ConditionalScopedLock<T>&);
    const ConditionalScopedLock<T>& operator=(const ConditionalScopedLock<T>&);

protected:
    T* mLocked;
};

} // namespace sead

#endif // SEAD_SCOPED_LOCK_H_

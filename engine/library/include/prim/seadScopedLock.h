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

} // namespace sead

#endif // SEAD_SCOPED_LOCK_H_

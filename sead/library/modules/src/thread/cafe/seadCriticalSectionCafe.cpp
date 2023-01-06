#include <thread/seadCriticalSection.h>

namespace sead {

CriticalSection::CriticalSection()
    : IDisposer()
    , mMutexInner()
{
    OSInitMutex(&mMutexInner);
}

CriticalSection::~CriticalSection()
{
}

void CriticalSection::lock()
{
    OSLockMutex(&mMutexInner);
}

bool CriticalSection::tryLock()
{
    return OSTryLockMutex(&mMutexInner);
}

void CriticalSection::unlock()
{
    OSUnlockMutex(&mMutexInner);
}

} // namespace sead

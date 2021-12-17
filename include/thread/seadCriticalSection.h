#ifndef SEAD_CRITICAL_SECTION_H_
#define SEAD_CRITICAL_SECTION_H_

#ifdef cafe
#include <cafe.h>
#endif // cafe

#include <heap/seadDisposer.h>

namespace sead {

class CriticalSection : public IDisposer
{
public:
    CriticalSection();

private:
    CriticalSection(const CriticalSection&);
    const CriticalSection& operator=(const CriticalSection&);

public:
    virtual ~CriticalSection();

    void lock();
    bool tryLock();
    void unlock();

protected:
#ifdef cafe
    OSMutex mMutexInner;
#else
    #error "Unknown platform"
#endif // cafe
};
#ifdef cafe
static_assert(sizeof(CriticalSection) == 0x3C, "sead::CriticalSection size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CRITICAL_SECTION_H_

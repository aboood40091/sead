#ifndef SEAD_EVENT_H_
#define SEAD_EVENT_H_

#ifdef cafe
#include <cafe.h>
#endif // cafe

#include <heap/seadDisposer.h>

namespace sead {

class Event : public IDisposer
{
public:
    Event(bool manual_reset);

private:
    Event(const Event&);
    const Event& operator=(const Event&);

public:
    virtual ~Event();

    void initialize(bool manual_reset);
    void wait();
    void setSignal();
    void resetSignal();

protected:
#ifdef cafe
    OSEvent mEventInner;
#else
    #error "Unknown platform"
#endif // cafe
};
#ifdef cafe
static_assert(sizeof(Event) == 0x34, "sead::Event size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_EVENT_H_

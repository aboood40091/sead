#include <thread/seadEvent.h>

namespace sead {

Event::Event(bool manual_reset)
    : IDisposer()
    , mEventInner()
{
    initialize(manual_reset);
}

Event::~Event()
{
}

void Event::initialize(bool manual_reset)
{
    OSInitEvent(&mEventInner, false, !manual_reset);
}

void Event::wait()
{
    OSWaitEvent(&mEventInner);
}

void Event::setSignal()
{
    OSSignalEvent(&mEventInner);
}

void Event::resetSignal()
{
    OSResetEvent(&mEventInner);
}

} // namespace sead

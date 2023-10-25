#include <thread/seadEvent.h>

namespace sead {

Event::Event(bool param)
    : IDisposer()
    , mEventInner()
{
    initialize(param);
}

Event::~Event()
{
}

void Event::initialize(bool param)
{
    OSInitEvent(&mEventInner, false, !param);
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

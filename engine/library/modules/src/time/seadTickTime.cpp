#include <time/seadTickTime.h>

namespace sead {

TickTime::TickTime()
{
    setNow();
}

TickSpan TickTime::diff(TickTime t) const
{
    return mTick - t.mTick;
}

TickSpan TickTime::diffToNow() const
{
    return TickTime().diff(*this);
}

TickTime& TickTime::operator+=(TickSpan rhs)
{
    mTick += rhs.toS64();
    return *this;
}

TickTime& TickTime::operator-=(TickSpan rhs)
{
    mTick -= rhs.toS64();
    return *this;
}

TickSpan operator-(TickTime lhs, TickTime rhs)
{
    return lhs.diff(rhs);
}

TickTime operator+(TickTime time, TickSpan span)
{
    TickTime t = time;
    t += span;
    return t;
}

TickTime operator-(TickTime time, TickSpan span)
{
    TickTime t = time;
    t -= span;
    return t;
}

} // namespace sead

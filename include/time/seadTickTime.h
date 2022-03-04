#ifndef SEAD_TICK_TIME_H_
#define SEAD_TICK_TIME_H_

#ifdef cafe
#include <cafe/os.h>
#endif // cafe

#include <time/seadTickSpan.h>

namespace sead {

class TickTime
{
public:
    TickTime();

#ifdef cafe
    void setNow()
    {
        mTick = OSGetSystemTime();
    }
#else
    void setNow();
#endif

    TickSpan diff(const TickTime& t) const;
    TickSpan diffToNow() const;

    TickTime& operator+=(TickSpan rhs);
    TickTime& operator-=(TickSpan rhs);

    u64 toU64() const
    {
        return mTick;
    }

private:
    u64 mTick;
};

TickTime operator+(TickTime time, TickSpan span);
TickTime operator-(TickTime time, TickSpan span);

TickSpan operator-(TickTime lhs, TickTime rhs);

}  // namespace sead

#endif // SEAD_TICK_TIME_H_

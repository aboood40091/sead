#ifndef SEAD_TICK_SPAN_H_
#define SEAD_TICK_SPAN_H_

#include <basis/seadTypes.h>

namespace sead {

class TickSpan
{
public:
    TickSpan()
        : mSpan(0)
    {
    }

    TickSpan(s64 tick)
        : mSpan(tick)
    {
    }

    s64 toS64() const
    {
        return mSpan;
    }

    s64 toNanoSeconds() const;

    // TODO
    s64 toMicroSeconds() const;
    s64 toMilliSeconds() const;
    s64 toSeconds() const;

    void setTicks(s64 tick)
    {
        mSpan = tick;
    }

    void setNanoSeconds(s64);

    void setMicroSeconds(s64 usec)
    {
        //SEAD_ASSERT(LLONG_MIN / cFrequency <= mSpan && mSpan < LLONG_MAX / cFrequency);
        mSpan = usec * cFrequency / 1000 / 1000;
    }

    void setMilliSeconds(s64 msec)
    {
        //SEAD_ASSERT(LLONG_MIN / cFrequency <= mSpan && mSpan < LLONG_MAX / cFrequency);
        mSpan = msec * cFrequency / 1000;
    }

    void setSeconds(s64 sec)
    {
        mSpan = sec * cFrequency;
    }

    const TickSpan& operator+=(TickSpan rhs)
    {
        mSpan += rhs.mSpan;
        return *this;
    }

    const TickSpan& operator-=(TickSpan rhs)
    {
        mSpan -= rhs.mSpan;
        return *this;
    }

    const TickSpan& operator*=(f32 s)
    {
        mSpan *= s;
        return *this;
    }

    const TickSpan& operator/=(f32 s)
    {
        mSpan /= s;
        return *this;
    }

    static TickSpan makeFromNanoSeconds(s64 nsec)
    {
        TickSpan span;
        span.setNanoSeconds(nsec);
        return span;
    }

    static TickSpan makeFromMicroSeconds(s64 usec)
    {
        TickSpan span;
        span.setMicroSeconds(usec);
        return span;
    }

    static TickSpan makeFromMilliSeconds(s64 msec)
    {
        TickSpan span;
        span.setMilliSeconds(msec);
        return span;
    }

    static TickSpan makeFromSeconds(s64 sec)
    {
        TickSpan span;
        span.setSeconds(sec);
        return span;
    }

private:
    s64 mSpan;

    static const s64 cFrequency;
};
#ifdef cafe
static_assert(sizeof(TickSpan) == 8, "sead::TickSpan size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_TICK_SPAN_H_

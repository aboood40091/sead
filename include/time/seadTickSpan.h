#ifndef SEAD_TICKSPAN_H_
#define SEAD_TICKSPAN_H_

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

private:
    s64 mSpan;

    static const s64 cFrequency;
};
#ifdef cafe
static_assert(sizeof(TickSpan) == 8, "sead::TickSpan size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_TICKSPAN_H_

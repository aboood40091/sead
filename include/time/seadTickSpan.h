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

} // namespace sead

#endif // SEAD_TICKSPAN_H_

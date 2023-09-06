#ifndef SEAD_PROCESS_METER_BAR_H_
#define SEAD_PROCESS_METER_BAR_H_

#include <gfx/seadColor.h>
#include <prim/seadNamable.h>
#include <time/seadTickTime.h>

namespace sead {

class ProcessMeterBarBase : public INamable
{
public:
    struct Section
    /* {
        TickTime begin;
        TickSpan span;
        Color4f color;
        s32 parent;
    } */;

public:
    ProcessMeterBarBase(/* Section* buffer, s32 section_num, */ const SafeString& name /*, const Color4f& color */)
        : INamable(name)
    {
    }

    ~ProcessMeterBarBase()
    {
    }

/*
protected:
    // ...
*/
};
#ifdef cafe
static_assert(sizeof(ProcessMeterBarBase) == 8, "sead::ProcessMeterBarBase size mismatch");
#endif // cafe

template <s32 N>
class MultiProcessMeterBar : public ProcessMeterBarBase
{
public:
    struct Section;

public:
    MultiProcessMeterBar(/* const SafeString& name, const Color4f& color */)
        : ProcessMeterBarBase(/* mBuffer, N, name */ "" /*, color */)
    {
    }

/*
private:
    Section mBuffer[N * 2];
*/
};
#ifdef cafe
static_assert(sizeof(MultiProcessMeterBar<32>) == 8, "sead::MultiProcessMeterBar size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_PROCESS_METER_BAR_H_

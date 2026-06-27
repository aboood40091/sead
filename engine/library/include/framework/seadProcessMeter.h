#ifndef SEAD_PROCESS_METER_H_
#define SEAD_PROCESS_METER_H_

#if 0 // defined(SEAD_TARGET_DEBUG)
#include <heap/seadTask.h>
#include <heap/seadTaskMgr.h>
#else
#include <heap/seadDisposer.h>
#endif

namespace sead {

class ProcessMeter
#if 0 // defined(SEAD_TARGET_DEBUG)
    : public Task
#endif
{
#if 0 // defined(SEAD_TARGET_DEBUG)
    SEAD_RTTI_OVERRIDE(ProcessMeter, Task)
    SEAD_TASK_SINGLETON_DISPOSER(ProcessMeter)
#else
    SEAD_SINGLETON_DISPOSER(ProcessMeter)
#endif

public:
#if 0 // defined(SEAD_TARGET_DEBUG)
    void measureBeginFrame();
    void measureEndFrame();
#else
    // These are no-ops in the release build
    void measureBeginFrame() { }
    void measureEndFrame() { }
#endif
};

} // namespace sead

#endif // SEAD_PROCESS_METER_H_

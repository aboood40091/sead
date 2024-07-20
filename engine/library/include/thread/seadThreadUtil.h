#ifndef SEAD_THREAD_UTIL_H_
#define SEAD_THREAD_UTIL_H_

#include <basis/seadTypes.h>

namespace sead {

class ThreadUtil
{
public:
    static s32 ConvertPrioritySeadToPlatform(s32 prio);
    static s32 ConvertPriorityPlatformToSead(s32 prio);
};

} // namespace sead

#endif // SEAD_THREAD_UTIL_H_

#ifndef SEAD_GLOBAL_RANDOM_H_
#define SEAD_GLOBAL_RANDOM_H_

#include <heap/seadDisposer.h>
#include <random/seadRandom.h>

namespace sead {

class GlobalRandom : public Random
{
    SEAD_SINGLETON_DISPOSER(GlobalRandom)

public:
    GlobalRandom()
        : Random()
    {
    }
};

} // namespace sead

#endif // SEAD_GLOBAL_RANDOM_H_

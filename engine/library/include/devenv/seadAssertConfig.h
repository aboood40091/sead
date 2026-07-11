#ifndef SEAD_ASSERT_CONFIG_H_
#define SEAD_ASSERT_CONFIG_H_

#include <prim/seadDelegateEventSlot.h>

namespace sead {

class AssertConfig
{
public:
    static void registerCallback(DelegateEvent<const char*>::Slot& slot);
    static void unregisterCallback(DelegateEvent<const char*>::Slot& slot);

    static void registerFinalCallback(IDelegate1<const char*>* callback);

    static void execCallbacks(const char* assert_msg);

private:
    static IDelegate1<const char*>* sFinalCallback;
    static DelegateEvent<const char*> sAssertEvent;
};

} // namespace sead

#endif // SEAD_ASSERT_CONFIG_H_

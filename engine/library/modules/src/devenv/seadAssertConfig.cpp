#include <devenv/seadAssertConfig.h>

namespace sead {

IDelegate1<const char*>* AssertConfig::sFinalCallback = nullptr;
DelegateEvent<const char*> AssertConfig::sAssertEvent;

void AssertConfig::registerCallback(DelegateEvent<const char*>::Slot& slot)
{
    sAssertEvent.connect(slot);
}

void AssertConfig::unregisterCallback(DelegateEvent<const char*>::Slot& slot)
{
    slot.release();
}

void AssertConfig::registerFinalCallback(IDelegate1<const char*>* callback)
{
    sFinalCallback = callback;
}

void AssertConfig::execCallbacks(const char* assert_msg)
{
    sAssertEvent.fire(assert_msg);

    if (sFinalCallback)
        (*sFinalCallback)(assert_msg);
}

} // namespace sead

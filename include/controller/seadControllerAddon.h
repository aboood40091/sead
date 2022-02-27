#ifndef SEAD_CONTROLLER_ADDON_H_
#define SEAD_CONTROLLER_ADDON_H_

#include <container/seadListImpl.h>
#include <controller/seadControllerDefine.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class Controller;

class ControllerAddon
{
    SEAD_RTTI_BASE(ControllerAddon)

public:
    ControllerAddon(Controller* controller)
        : mListNode()
        , mId(ControllerDefine::cAddon_Null)
        , mController(controller)
    {
        //SEAD_ASSERT(controller);
    }

    virtual ~ControllerAddon()
    {
    }

    virtual bool calc() = 0;

protected:
    ListNode mListNode;
    ControllerDefine::AddonId mId;
    Controller* mController;

    friend class Controller;
};
#ifdef cafe
static_assert(sizeof(ControllerAddon) == 0x14, "sead::ControllerAddon size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_ADDON_H_

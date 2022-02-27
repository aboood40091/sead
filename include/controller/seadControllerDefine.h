#ifndef SEAD_CONTROLLER_DEFINE_H_
#define SEAD_CONTROLLER_DEFINE_H_

namespace sead {

class ControllerDefine
{
public:
    enum ControllerId
    {
        cController_Null       = 0,
        cController_Win        = 1,
        cController_Ctr        = 2,
        cController_CtrDebug   = 3,
        cController_WiiRemote  = 4,
        cController_CafeDebug  = 5,
        cController_WinDRC     = 6,
        cController_CafeRemote = 7,
        cController_CafeDRC    = 8,
        cController_Merge      = 9,
        cController_UserDefine = 10
    };

    enum AddonId
    {
        cAddon_Null          = 0,
        cAddon_Accelerometer = 1,
        cAddon_PatternRumble = 2,
        cAddon_UserDefine    = 3
    };
};

} // namespace sead

#endif // SEAD_CONTROLLER_DEFINE_H_

#ifndef SEAD_GAME_FRAMEWORK_H_
#define SEAD_GAME_FRAMEWORK_H_

#include <framework/seadFramework.h>
#include <framework/seadProcessMeterBar.h>

namespace sead {

class InfLoopCheckerThread;

class GameFramework : public Framework
{
    SEAD_RTTI_OVERRIDE(GameFramework, Framework)

public:
    static void initialize(const InitializeArg& arg);

public:
    GameFramework();
    virtual ~GameFramework();

    virtual void createSystemTasks(TaskBase*, const CreateSystemTaskArg&);

    virtual void createControllerMgr(TaskBase*);
    virtual void createHostIOMgr(TaskBase*, HostIOMgr::Parameter*);
    virtual void createProcessMeter(TaskBase* root_task);
    virtual void createSeadMenuMgr(TaskBase*);
    virtual void createInfLoopChecker(TaskBase*, const TickSpan&);
    virtual f32 calcFps() = 0;
    virtual void saveScreenShot(const SafeString&) { }
    virtual bool isScreenShotBusy() { return false; }

protected:
    virtual void waitStartDisplayLoop_();

protected:
    enum DisplayState
    {
        cDisplayState_Hide = 0,
        cDisplayState_Ready,
        cDisplayState_Show
    };

    DisplayState mDisplayState;
    MultiProcessMeterBar<32> mCalcMeter;
    MultiProcessMeterBar<32> mDrawMeter;
    MultiProcessMeterBar<32> mGPUMeter;
    InfLoopCheckerThread* mCheckerThread;
};
#ifdef cafe
static_assert(sizeof(GameFramework) == 0x48, "sead::GameFramework size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_GAME_FRAMEWORK_H_

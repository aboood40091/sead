#ifndef SEAD_AUDIOSYSTEM_H_
#define SEAD_AUDIOSYSTEM_H_

#include <audio/seadAudioGlobal.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class AudioFx;
class ISoundFrameCallback;

class AudioSystem
{
    SEAD_RTTI_BASE(AudioSystem)

public:
    AudioSystem() { }
    virtual ~AudioSystem() { }

    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual bool setOutputMode(AudioGlobal::OutputMode) = 0;
    virtual AudioGlobal::OutputMode getOutputMode() const = 0;
    virtual bool appendEffect(AudioGlobal::AuxBus, AudioFx*) = 0;
    virtual void clearEffect(AudioGlobal::AuxBus, s32) = 0;
    virtual bool isFinishedClearEffect(AudioGlobal::AuxBus) = 0;
    virtual void appendSoundFrameCallback(ISoundFrameCallback&) = 0;
    virtual void removeSoundFrameCallback(ISoundFrameCallback&) = 0;
    virtual void clearSoundFrameCallback() = 0;

protected:
    virtual void initializeSdk_() = 0;
    virtual void finalizeSdk_() = 0;
    virtual void initializeNw_() = 0;
};
#ifdef cafe
static_assert(sizeof(AudioSystem) == 4, "sead::AudioSystem size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_AUDIOSYSTEM_H_

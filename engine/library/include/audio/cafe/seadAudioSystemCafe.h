#ifndef SEAD_AUDIOSYSTEM_CAFE_H_
#define SEAD_AUDIOSYSTEM_CAFE_H_

#include <audio/seadAudioSystem.h>

namespace sead {

class AudioSystemCafe : public AudioSystem
{
    SEAD_RTTI_OVERRIDE(AudioSystemCafe, AudioSystem)

public:
    AudioSystemCafe();
    virtual ~AudioSystemCafe() { }

    virtual void initialize();
    virtual void finalize();
    virtual bool setOutputMode(AudioGlobal::OutputMode);
    virtual AudioGlobal::OutputMode getOutputMode() const;
    virtual bool appendEffect(AudioGlobal::AuxBus, AudioFx*);
    virtual void clearEffect(AudioGlobal::AuxBus, s32);
    virtual bool isFinishedClearEffect(AudioGlobal::AuxBus);
    virtual void appendSoundFrameCallback(ISoundFrameCallback&);
    virtual void removeSoundFrameCallback(ISoundFrameCallback&);
    virtual void clearSoundFrameCallback();

protected:
    virtual void initializeSdk_();
    virtual void finalizeSdk_();
    virtual void initializeNw_();

public:
    void forceQuit();
};

} // namespace sead

#endif // SEAD_AUDIOSYSTEM_CAFE_H_

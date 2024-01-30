#ifndef SEAD_AUDIO_PLAYER_H_
#define SEAD_AUDIO_PLAYER_H_

#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class SoundHandle;

class AudioPlayer
{
    SEAD_RTTI_BASE(AudioPlayer)

public:
    AudioPlayer()
    {
    }

    virtual ~AudioPlayer()
    {
    }

    virtual void initialize();
    virtual void finalize();
    virtual void calc();
    virtual bool startSound(SoundHandle*, u32);
    virtual bool startSound(SoundHandle*, const char*);
    virtual bool holdSound(SoundHandle*, u32);
    virtual bool holdSound(SoundHandle*, const char*);
    virtual u32 getSoundCount() const;
    virtual const char* getSoundName(u32) const;
    virtual u32 getSoundId(const char*) const;
};
#ifdef cafe
static_assert(sizeof(AudioPlayer) == 4, "sead::AudioPlayer size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_AUDIO_PLAYER_H_

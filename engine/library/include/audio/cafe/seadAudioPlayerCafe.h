#ifndef SEAD_AUDIO_PLAYER_CAFE_H_
#define SEAD_AUDIO_PLAYER_CAFE_H_

#include <audio/seadAudioPlayer.h>

#include <nw/snd/snd_SoundArchivePlayer.h>

namespace sead {

class SoundHandle;

class AudioPlayerCafe : public AudioPlayer, public nw::snd::SoundArchivePlayer
{
    SEAD_RTTI_OVERRIDE(AudioPlayerCafe, AudioPlayer)

public:
    AudioPlayerCafe();
    virtual ~AudioPlayerCafe();

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

protected:
    virtual nw::snd::SoundStartable::StartResult detail_SetupSound(nw::snd::SoundHandle* handle, u32 soundId, bool holdFlag, const nw::snd::SoundStartable::StartInfo* startInfo);

private:
    u32 _c8[(0x134 - 0xC8) / sizeof(u32)];
};
#ifdef cafe
static_assert(sizeof(AudioPlayerCafe) == 0x134, "sead::AudioPlayerCafe size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_AUDIO_PLAYER_CAFE_H_

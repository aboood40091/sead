#ifndef SEAD_AUDIO_SOUND_HEAP_CAFE_H_
#define SEAD_AUDIO_SOUND_HEAP_CAFE_H_

#include <basis/seadTypes.h>

#include <nw/snd/snd_SoundArchive.h>
#include <nw/snd/snd_SoundDataManager.h>
#include <nw/snd/snd_SoundHeap.h>

namespace sead {

class Heap;

class AudioSoundHeapCafe : public nw::snd::SoundHeap
{
public:
    AudioSoundHeapCafe(size_t size, Heap* heap);
    ~AudioSoundHeapCafe();

    void setSoundDataManagement(nw::snd::SoundDataManager& soundDataMgr, nw::snd::SoundArchive& soundArchive);

    bool isCrossingMemoryBoundary() const;

private:
    void create_(size_t size, Heap* heap);

private:
    u8* mBuffer;
    nw::snd::SoundDataManager* mSoundDataMgr;
    nw::snd::SoundArchive* mSoundArchive;
};
static_assert(sizeof(AudioSoundHeapCafe) == 0x50);

} // namespace sead

#endif // SEAD_AUDIO_SOUND_HEAP_CAFE_H_

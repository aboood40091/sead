#ifndef SEAD_AUDIOMGR_H_
#define SEAD_AUDIOMGR_H_

#include <container/seadOffsetList.h>
#include <heap/seadDisposer.h>
#include <hostio/seadHostIODummy.h>

namespace sead {

class AudioPlayer;
class AudioResetter;
class AudioResourceLoader;
class AudioSubsetBase;
class AudioSystem;
class Heap;

class AudioMgr : public hostio::Node
{
    SEAD_SINGLETON_DISPOSER(AudioMgr)

public:
    AudioMgr();
    virtual ~AudioMgr();

    AudioSystem* getAudioSystem() const
    {
        return mAudioSystem;
    }

protected:
    AudioSystem* mAudioSystem;
    AudioResetter* mResetter;
    AudioPlayer* mPlayer;
    AudioResourceLoader* mResourceLoader;
    OffsetList<AudioSubsetBase> mSubsets;
    Heap* mHeap;
    bool mIsPrepared;
    bool mUseDefaultAudioSystem;
    bool mUseDefaultResetter;
    bool mUseDefaultPlayer;
};
#ifdef cafe
static_assert(sizeof(AudioMgr) == 0x3C, "sead::AudioMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_AUDIOMGR_H_

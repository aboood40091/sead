#ifndef SEAD_AUDIO_SETTING_PARAMETER_H_
#define SEAD_AUDIO_SETTING_PARAMETER_H_

#include <container/seadOffsetList.h>
#include <framework/seadTaskParameter.h>

namespace sead {

class AudioMgr;
class AudioPlayer;
class AudioResetter;
class AudioResourceLoader;
class AudioSubsetBase;
class AudioSystem;

class AudioSettingParameter : public TaskParameter
{
    SEAD_RTTI_OVERRIDE(AudioSettingParameter, TaskParameter)

public:
    AudioSettingParameter();

    void setAudioSystem(AudioSystem* audio_system);
    void setResetter(AudioResetter* resetter);
    void setPlayer(AudioPlayer* player);
    void setResourceLoader(AudioResourceLoader* resource_loader);

    void appendSubset(AudioSubsetBase* subset);

private:
    AudioSystem* mAudioSystem;
    AudioResetter* mResetter;
    AudioPlayer* mPlayer;
    AudioResourceLoader* mResourceLoader;
    OffsetList<AudioSubsetBase> mSubsets;

    friend class AudioMgr;
};
#ifdef cafe
static_assert(sizeof(AudioSettingParameter) == 0x24, "sead::AudioSettingParameter size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_AUDIO_SETTING_PARAMETER_H_

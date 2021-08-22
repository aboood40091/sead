#ifndef SEAD_DECOMPRESSOR_H_
#define SEAD_DECOMPRESSOR_H_

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <prim/seadSafeString.h>
#include <resource/seadResource.h>
#include <resource/seadResourceMgr.h>

namespace sead {

class Decompressor : public TListNode<Decompressor*>, public IDisposer
{
public:
    Decompressor(const SafeString& default_ext)
        : TListNode<Decompressor*>(this)
        , IDisposer()
        , mExt(default_ext)
    {
    }

    virtual ~Decompressor()
    {
        if (ResourceMgr::sInstance != NULL)
            ResourceMgr::sInstance->unregisterDecompressor(this);
    }

    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& arg, Resource* res, u32* out_size, u32* out_buffer_size, bool* out_need_delete) = 0;

private:
    friend class ResourceMgr;

    FixedSafeString<32> mExt;
};

} // namespace sead

#endif // SEAD_DECOMPRESSOR_H_

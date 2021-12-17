#ifndef SEAD_SZS_DECOMPRESSOR_H_
#define SEAD_SZS_DECOMPRESSOR_H_

#include <resource/seadDecompressor.h>
#include <resource/seadResource.h>
#include <resource/seadResourceMgr.h>

namespace sead {

class SZSDecompressor : public Decompressor
{
public:
    enum Step
    {
        cStepNormal = 0,
        cStepShort = 1,
        cStepLong = 2,
    };

    struct DecompContext
    {
        DecompContext();
        DecompContext(void* dst);

        void initialize(void* dst);

        u8* destp;
        s32 destCount;
        s32 forceDestCount;
        u8 flagMask;
        u8 flags;
        u8 packHigh;
        Step step;
        u16 lzOffset;
        u8 headerSize;
    };
#ifdef cafe
    static_assert(sizeof(DecompContext) == 0x18, "sead::SZSDecompressor::DecompContext size mismatch");
#endif // cafe

public:
    SZSDecompressor(u32 work_size, u8* work_buffer);
    virtual ~SZSDecompressor() { }

    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& arg, Resource* res, u32* out_size, u32* out_buffer_size, bool* out_need_delete);

    static inline u32 getHeaderSize() { return 0x10; }
    void setWorkSize(u32 work_size);

private:
    static u32 getDecompAlignment(const void* header);
    static u32 getDecompSize(const void* header);
    static s32 readHeader_(DecompContext* context, const u8* srcp, u32 src_size);
    static s32 streamDecomp(DecompContext* context, const void* src, u32 len);
    static s32 decomp(void* dst, u32 dst_size, const void* src, u32 src_size);

    u32 mWorkSize;
    u8* mWorkBuffer;
};
#ifdef cafe
static_assert(sizeof(SZSDecompressor) == 0x58, "sead::SZSDecompressor size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_SZS_DECOMPRESSOR_H_

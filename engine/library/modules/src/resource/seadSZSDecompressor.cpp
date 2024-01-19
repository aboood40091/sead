#include <filedevice/seadFileDeviceMgr.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <math/seadMathCalcCommon.h>
#include <prim/seadEndian.h>
#include <prim/seadSafeString.h>
#include <resource/seadSZSDecompressor.h>

namespace {

#ifdef cafe
__attribute__((aligned(0x20))) s32 decodeSZSCafeAsm_(void* dst, const void* src)
{
    asm ("lwz r5, 0x4(r4)\n");
    asm ("li r11, 0x20\n");
    asm ("li r6, 0\n");
    asm ("mr r0, r5\n");
    asm ("addi r4, r4, 0xf\n");
    asm ("subi r3, r3, 1\n");
    asm ("cmpwi r5, 0x132\n");
    asm ("ble _final_decloop0\n");
    asm ("subi r5, r5, 0x132\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");

    asm ("_decloop0: rlwinm. r6, r6, 0x1f, 1, 0x1f\n");
    asm ("bne _decloop1\n");
    asm ("lbzu r7, 1(r4)\n");
    asm ("li r6, 0x80\n");

    asm ("_decloop1: and. r8, r6, r7\n");
    asm ("lbzu r8, 1(r4)\n");
    asm ("beq _decloop2\n");
    asm ("andi. r9, r3, 0x1f\n");
    asm ("bne _decloop1x\n");
    asm ("dcbz r11, r3\n");

    asm ("_decloop1x: subic. r5, r5, 1\n");
    asm ("stbu r8, 1(r3)\n");
    asm ("bne _decloop0\n");
    asm ("b _decloop8\n");

    asm ("_decloop2: lbzu r9, 1(r4)\n");
    asm ("rlwinm. r10, r8, 0x1c, 4, 0x1f\n");
    asm ("bne _decloop3\n");
    asm ("lbzu r10, 1(r4)\n");
    asm ("addi r10, r10, 0x10\n");

    asm ("_decloop3: addi r10, r10, 2\n");
    asm ("rlwimi r9, r8, 8, 0x14, 0x17\n");
    asm ("subf r5, r10, r5\n");
    asm ("subf r8, r9, r3\n");
    asm ("mtspr CTR, r10\n");
    asm ("addi r8, r8, 1\n");

    asm ("_decloop4: andi. r9, r3, 0x1f\n");
    asm ("lbz r9, -1(r8)\n");
    asm ("addi r8, r8, 1\n");
    asm ("bne _decloop5\n");
    asm ("dcbz r11, r3\n");

    asm ("_decloop5: stbu r9, 1(r3)\n");
    asm ("bdnz _decloop4\n");
    asm ("cmpwi r5, 0\n");
    asm ("bgt _decloop0\n");

    asm ("_decloop8: addi r5, r5, 0x132\n");
    asm ("cmpwi r5, 0\n");
    asm ("ble _final_decloop8\n");

    asm ("_final_decloop0: rlwinm. r6, r6, 0x1f, 1, 0x1f\n");
    asm ("bne _final_decloop1\n");
    asm ("lbzu r7, 1(r4)\n");
    asm ("li r6, 0x80\n");

    asm ("_final_decloop1: and. r8, r6, r7\n");
    asm ("lbzu r8, 1(r4)\n");
    asm ("beq _final_decloop2\n");
    asm ("subic. r5, r5, 1\n");
    asm ("stbu r8, 1(r3)\n");
    asm ("bne _final_decloop0\n");
    asm ("b _final_decloop8\n");

    asm ("_final_decloop2: lbzu r9, 1(r4)\n");
    asm ("rlwinm. r10, r8, 0x1c, 4, 0x1f\n");
    asm ("bne _final_decloop3\n");
    asm ("lbzu r10, 1(r4)\n");
    asm ("addi r10, r10, 0x10\n");

    asm ("_final_decloop3: addi r10, r10, 2\n");
    asm ("rlwimi r9, r8, 8, 0x14, 0x17\n");
    asm ("subf. r5, r10, r5\n");
    asm ("blt _final_decloop8\n");
    asm ("subf r8, r9, r3\n");
    asm ("mtspr CTR, r10\n");
    asm ("addi r8, r8, 1\n");

    asm ("_final_decloop4: lbz r9, -1(r8)\n");
    asm ("addi r8, r8, 1\n");
    asm ("stbu r9, 1(r3)\n");
    asm ("bdnz _final_decloop4\n");
    asm ("cmpwi r5, 0\n");
    asm ("bgt _final_decloop0\n");

    s32 register error asm("r3");
    asm ("_final_decloop8: mr %0, r0\n" : "=r"(error));
    asm ("blr");

    return error;
}
#endif // cafe

} // namespace {no-name}

namespace sead {

SZSDecompressor::DecompContext::DecompContext()
{
    initialize(nullptr);
}

SZSDecompressor::DecompContext::DecompContext(void* dst)
{
  //SEAD_ASSERT(dst);
    initialize(dst);
}

void
SZSDecompressor::DecompContext::initialize(void* dst)
{
    destp = static_cast<u8*>(dst);
    destCount = 0;
    headerSize = SZSDecompressor::getHeaderSize();
    forceDestCount = 0;
    flagMask = 0;
    flags = 0;
    packHigh = 0;
    step = SZSDecompressor::cStepNormal;
    lzOffset = 0;
}

SZSDecompressor::SZSDecompressor(u32 work_size, u8* work_buffer)
    : Decompressor("szs")
{
    if (work_buffer == nullptr)
    {
        mWorkSize = Mathu::roundUpPow2(work_size, FileDevice::cBufferMinAlignment);
        mWorkBuffer = nullptr;
    }

    else
    {
        //SEAD_ASSERT_MSG(Mathu::isMultiplePow2(work_size, FileDevice::cBufferMinAlignment), "work_size[%d] must be multiple of FileDevice::cBufferMinAlignment", work_size);

        //SEAD_ASSERT_MSG(PtrUtil::isAlignedPow2(work_buffer, FileDevice::cBufferMinAlignment), "work_buffer[0x%x] must be aligned to FileDevice::cBufferMinAlignment");

        mWorkSize = work_size;
        mWorkBuffer = work_buffer;
    }
}

u8*
SZSDecompressor::tryDecompFromDevice(
    const ResourceMgr::LoadArg& arg, Resource* res,
    u32* out_size, u32* out_buffer_size, bool* out_need_delete
)
{
    Heap* heap = arg.load_data_heap;
    if (heap == nullptr)
        heap = HeapMgr::instance()->getCurrentHeap();

    FileHandle handle;
    FileDevice* device = nullptr;
    if (arg.device != nullptr)
        device = arg.device->tryOpen(&handle, arg.path, FileDevice::cFileOpenFlag_ReadOnly, arg.div_size);
    else
        device = FileDeviceMgr::instance()->tryOpen(&handle, arg.path, FileDevice::cFileOpenFlag_ReadOnly, arg.div_size);

    if (device == nullptr)
        return nullptr;

    u8* work_buf = mWorkBuffer;
    if (work_buf == nullptr)
        work_buf = new(heap, -FileDevice::cBufferMinAlignment) u8[mWorkSize];

    if (work_buf == nullptr)
    {
        //SEAD_ASSERT_MSG(false, "cannot alloc work buf");
        return nullptr;
    }

    u32 decomp_size;
    u32 decomp_alignment;

    u32 read_size = handle.read(work_buf, mWorkSize);
    if (read_size < getHeaderSize())
    {
        //SEAD_ASSERT_MSG(false, "Invalid header size.");

        if (mWorkBuffer == nullptr)
            delete[] work_buf;

        return nullptr;
    }

    decomp_size = getDecompSize(work_buf);
    //SEAD_ASSERT(decomp_size > 0);
    decomp_alignment = getDecompAlignment(work_buf);
    //SEAD_ASSERT_MSG(decomp_alignment == 0 || Mathu::isPow2(decomp_alignment), "decomp_alignment[%d] must be power of 2.", decomp_alignment);

    if (decomp_size > arg.load_data_buffer_size && arg.load_data_buffer_size != 0)
        decomp_size = arg.load_data_buffer_size;

    u32 buffer_size = Mathi::roundUpPow2(decomp_size, 0x20);

    u8* dst_buf = arg.load_data_buffer;
    bool need_delete = false;

    if (dst_buf == nullptr)
    {
        DirectResource* d_res = DynamicCast<DirectResource>(res);
        s32 align = 0;

        if (d_res != nullptr)
        {
            if (arg.load_data_alignment != 0)
            {
                align = Mathi::max(arg.load_data_alignment, 0x20);
                //SEAD_ASSERT_MSG(decomp_alignment == 0 || align % decomp_alignment == 0, "load_data_alignment[%d] doesn\'t meet decomp_alignment[%d].", arg.load_data_alignment, decomp_alignment);
            }
            else
            {
                if (decomp_alignment == 0)
                    align = d_res->getLoadDataAlignment();
                else
                    align = decomp_alignment;

                align = Mathi::sign(arg.instance_alignment) * Mathi::max(align, 0x20);
            }
        }
        else
        {
            align = -Mathi::sign(arg.instance_alignment) * 0x20;
        }

        dst_buf = new(heap, align) u8[buffer_size];

        if (dst_buf == nullptr)
        {
            //SEAD_ASSERT_MSG(false, "cannot alloc dst buf");

            if (mWorkBuffer == nullptr)
                delete[] work_buf;

            return nullptr;
        }
        else
        {
            need_delete = true;
        }
    }
    else
    {
        //SEAD_ASSERT(arg.load_data_buffer_size >= decomp_size);

        //SEAD_ASSERT_MSG(decomp_alignment == 0 || PtrUtil::isAlignedPow2(dst_buf, decomp_alignment), "load_data_buffer is not aligned with decomp_alignment[%d]", decomp_alignment);
    }

    if (read_size < mWorkSize)
    {
        s32 dret = decomp(dst_buf, buffer_size, work_buf, mWorkSize);
        if (dret < 0)
        {
            //SEAD_ASSERT_MSG(false, "decompSZS returned error(%d).", dret);

            if (need_delete)
                delete[] dst_buf;

            if (mWorkBuffer == nullptr)
                delete[] work_buf;

            return nullptr;
        }
    }
    else
    {
        DecompContext context(dst_buf);
        context.forceDestCount = decomp_size;

        while (read_size > 0)
        {
            s32 dret = streamDecomp(&context, work_buf, read_size);
            if (dret == 0)
                break;

            if (dret < 0)
            {
                //SEAD_ASSERT_MSG(false, "streamDecompSZS returned error(%d).", dret);

                if (need_delete)
                    delete[] dst_buf;

                if (mWorkBuffer == nullptr)
                    delete[] work_buf;

                return nullptr;
            }

            read_size = handle.read(work_buf, mWorkSize);
        }
    }

    if (mWorkBuffer == nullptr)
        delete[] work_buf;

    if (out_size != nullptr)
        *out_size = decomp_size;

    if (out_buffer_size != nullptr)
        *out_buffer_size = buffer_size;

    if (out_need_delete != nullptr)
        *out_need_delete = need_delete;

    return dst_buf;
}

void
SZSDecompressor::setWorkSize(u32 work_size)
{
    if (mWorkBuffer == nullptr)
        mWorkSize = Mathu::roundUpPow2(work_size, FileDevice::cBufferMinAlignment);

    else
    {
        //SEAD_ASSERT_MSG(false, "cannot change work_size when work buffer is given");
    }
}

u32
SZSDecompressor::getDecompAlignment(const void* header)
{
    return Endian::toHostU32(Endian::cBig, *(u32*)(header + 8));
}

u32
SZSDecompressor::getDecompSize(const void* header)
{
    return Endian::toHostU32(Endian::cBig, *(u32*)(header + 4));
}

s32
SZSDecompressor::readHeader_(DecompContext* context, const u8* srcp, u32 src_size)
{
    s32 readLen = 0;

    while (context->headerSize > 0)
    {
        context->headerSize--;

        if (context->headerSize == 0xF)
        {
            if (*srcp != 'Y')
                return -1;
        }
        else if (context->headerSize == 0xE)
        {
            if (*srcp != 'a')
                return -1;
        }
        else if (context->headerSize == 0xD)
        {
            if (*srcp != 'z')
                return -1;
        }
        else if (context->headerSize == 0xC)
        {
            if (*srcp != '0')
                return -1;
        }
        else if (context->headerSize >= 8)
        {
            context->destCount |= static_cast<u32>(*srcp) << ((static_cast<u32>(context->headerSize) - 8) << 3);
        }

        srcp++; readLen++; src_size--;
        if (src_size == 0 && context->headerSize > 0)
            return readLen;
    }

    if (context->forceDestCount > 0 && context->forceDestCount < context->destCount)
        context->destCount = context->forceDestCount;

    return readLen;
}

s32
SZSDecompressor::streamDecomp(DecompContext* context, const void* src, u32 len)
{
    //SEAD_ASSERT(context);
    //SEAD_ASSERT(src);

    const u8* srcp = static_cast<const u8*>(src);

    if (context->headerSize > 0)
    {
        s32 read_len = readHeader_(context, srcp, len);
        if (read_len < 0)
            return read_len;

        srcp += read_len; len -= read_len;
        if (len == 0)
        {
            if (context->headerSize == 0)
                return context->destCount;

            return -1;
        }
    }

    while (context->destCount > 0)
    {
        if (context->step == cStepLong)
        {
            u32 lzLength = (*srcp++) + 0x12; len--;
            if (lzLength > context->destCount)
            {
                if (context->forceDestCount == 0)
                    return -2;

                lzLength = static_cast<u16>(context->destCount);
            }

            context->destCount -= lzLength;

            do
            {
                *context->destp = *(context->destp - context->lzOffset);
                context->destp++;
            }
            while (--lzLength != 0);

            context->step = SZSDecompressor::cStepNormal;
        }
        else if (context->step == cStepShort)
        {
            u32 pack = static_cast<u32>(context->packHigh) << 8 | *srcp++; len--;
            context->lzOffset = (pack & 0xFFFu) + 1;

            u32 lzLength = pack >> 12;
            if (lzLength == 0)
                context->step = cStepLong;

            else
            {
                lzLength  += 2;
                if (lzLength > context->destCount)
                {
                    if (context->forceDestCount == 0)
                        return -2;

                    lzLength = static_cast<u16>(context->destCount);
                }

                context->destCount -= lzLength;

                do
                {
                    *context->destp = *(context->destp - context->lzOffset);
                    context->destp++;
                }
                while (--lzLength != 0);

                context->step = SZSDecompressor::cStepNormal;
            }
        }
        else
        {
            if (context->flagMask == 0)
            {
                context->flags = *srcp++; len--;
                context->flagMask = 0x80;
                if (len == 0)
                    break;
            }

            if ((context->flags & context->flagMask) != 0)
            {
                *context->destp++ = *srcp++;
                context->destCount--;
            }
            else
            {
                context->packHigh = *srcp++;
                context->step = cStepShort;
            }

            len--;
            context->flagMask >>= 1;
        }

        if (len == 0)
            break;
    }

    if (context->destCount == 0 && context->forceDestCount == 0 && len > 0x20)
        return -1;

    return context->destCount;
}

s32
SZSDecompressor::decomp(void* dst, u32 dst_size, const void* src, u32 src_size)
{
    //SEAD_ASSERT(src_size >= getHeaderSize());
    //SEAD_ASSERT(dst);
    //SEAD_ASSERT(src);

    //
    //SEAD_ASSERT_MSG(((uintptr_t)dst & 0x20-1u) == 0, "dst[0x%x] must be aligned to 32.", (uintptr_t)dst);
    //SEAD_ASSERT_MSG((dst_size & 0x20-1u) == 0, "dst_size[%u] must be multiple of 32.", dst_size);

    if (Endian::toHostU32(Endian::cBig, *static_cast<const u32*>(src)) != 0x59617A30) // Yaz0
        return -1;

    u32 decomp_size = getDecompSize(src);
    if (dst_size < decomp_size)
        return -2;

#ifdef cafe
    return decodeSZSCafeAsm_(dst, src);
#else
    #error "Unknown platform"
#endif // cafe
}

} // namespace sead

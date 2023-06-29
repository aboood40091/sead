#pragma once

#include <common/aglVertexEnum.h>

#include <cafe/gx2.h>

namespace agl {

class IndexStream
{
public:
    typedef GX2PrimitiveType PrimitiveType; // Idk the actual type

public:
    IndexStream();
    virtual ~IndexStream();

    PrimitiveType getPrimitiveType() const { return mPrimitiveType; }
    void setPrimitiveType(PrimitiveType primitive_type) { mPrimitiveType = primitive_type; }

    void setUpStream(const u16* addr, u32 count)
    {
        setUpStream_(addr, cIndexStreamFormat_u16, count);
    }

    void setUpStream(const u16* addr, u32 count, PrimitiveType primitive_type)
    {
        setUpStream_(addr, cIndexStreamFormat_u16, count);
        setPrimitiveType(primitive_type);
    }

    void setUpStream(const u32* addr, u32 count)
    {
        setUpStream_(addr, cIndexStreamFormat_u32, count);
    }

    void setUpStream(const u32* addr, u32 count, PrimitiveType primitive_type)
    {
        setUpStream_(addr, cIndexStreamFormat_u32, count);
        setPrimitiveType(primitive_type);
    }

    const void* getBufferPtr(u32 start) const
    {
        u32 offset = mFormat == cIndexStreamFormat_u16
            ? sizeof(u16) * start
            : sizeof(u32) * start;

        return static_cast<const u8*>(mpBuffer) + offset;
    }

    u32 getCount() const { return mCount; }

    void draw() const;
    void draw(u32 start, u32 count) const;

    void drawInstanced(u32 instance_num) const;
    void drawInstanced(u32 start, u32 count, u32 instance_num) const;

    void drawInstanced(u32 instance_num, u32 base_vertex) const;
    void drawInstanced(u32 start, u32 count, u32 instance_num, u32 base_vertex) const;

private:
    void setUpStream_(const void* addr, IndexStreamFormat format, u32 count);
    void cleanUp_();

private:
    IndexStreamFormat mFormat;
    PrimitiveType mPrimitiveType;
    const void* mpBuffer;
    u32 mCount;
    u32 mStride;
};
static_assert(sizeof(IndexStream) == 0x18, "agl::IndexStream size mismatch");

}

#ifdef __cplusplus

#include <common/aglIndexStream.hpp>

#endif // __cplusplus

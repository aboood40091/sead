#pragma once

namespace agl {

inline void
IndexStream::draw() const
{
    drawInstanced(1, 0);
}

inline void
IndexStream::draw(u32 start, u32 count) const
{
    drawInstanced(start, count, 1, 0);
}

inline void
IndexStream::drawInstanced(u32 instance_num) const
{
    drawInstanced(instance_num, 0);
}

inline void
IndexStream::drawInstanced(u32 start, u32 count, u32 instance_num) const
{
    drawInstanced(start, count, instance_num, 0);
}

inline void
IndexStream::drawInstanced(u32 instance_num, u32 base_vertex) const
{
    drawInstanced(0, getCount(), instance_num, base_vertex);
}

inline void
IndexStream::drawInstanced(u32 start, u32 count, u32 instance_num, u32 base_vertex) const
{
    if (count > 0)
    {
        // SEAD_ASSERT(( start + count ) <= getCount());
        GX2DrawIndexedEx(mPrimitiveType, count, GX2IndexFormat(mFormat), getBufferPtr(start), base_vertex, instance_num);
    }
}

}

#ifndef SEAD_STREAM_SRC_H_
#define SEAD_STREAM_SRC_H_

#include <basis/seadTypes.h>

namespace sead {

class StreamSrc
{
public:
    virtual u32 read(void* dst, u32 size) = 0;
    virtual u32 write(const void* src, u32 size) = 0;
    virtual u32 skip(s32 byte) = 0;
    virtual void rewind() = 0;
    virtual bool isEOF() = 0;
    virtual bool flush() { return true; }
};

} // namespace sead

#endif // SEAD_STREAM_SRC_H_

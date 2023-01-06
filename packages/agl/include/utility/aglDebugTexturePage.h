#pragma once

#include <container/seadBuffer.h>
#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <prim/seadSafeString.h>

namespace agl { namespace utl {

class DebugTexturePage : public sead::IDisposer, public sead::TListNode<DebugTexturePage*>
{
public:
    class Context;

public:
    DebugTexturePage();

    virtual ~DebugTexturePage()
    {
        cleanUp();
    }

    void setUp(u32 ctx_num, const sead::SafeString&, sead::Heap*);
    void cleanUp();

private:
    sead::FixedSafeString<128> _20;
    u8 _ac;
    u32 mCurrentContext;
    sead::Buffer<Context> mContext;
    sead::FixedSafeString<128> _bc;
    sead::FixedSafeString<128> _148;
    u8 _1d4;
    u8 _1d5;
    u8 _1d6;
    u32 _1d8;
};
static_assert(sizeof(DebugTexturePage) == 0x1DC, "agl::utl::DebugTexturePage size mismatch");

} }

#include <utility/aglDebugTexturePage.h>

namespace agl { namespace utl {

DebugTexturePage::DebugTexturePage()
    : sead::IDisposer()
    , sead::TListNode<DebugTexturePage*>(this)
    , _20("")
    , _ac(0)
    , mCurrentContext(0)
    , mContext()
    , _bc()
    , _148()
    , _1d4(1)
    , _1d5(0)
    , _1d6(0)
    , _1d8(0)
{
}

void DebugTexturePage::setUp(u32 ctx_num, const sead::SafeString&, sead::Heap*)
{
}

void DebugTexturePage::cleanUp()
{
}

} }

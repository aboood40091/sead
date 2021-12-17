#include <prim/seadSafeString.h>
#include <resource/seadArchiveRes.h>

namespace sead {

void ArchiveRes::doCreate_(u8* buf, u32, Heap*)
{
    mEnable = prepareArchive_(buf);
}

} // namespace sead

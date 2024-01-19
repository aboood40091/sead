#include <filedevice/seadArchiveFileDevice.h>
#include <prim/seadSafeString.h>
#include <resource/seadArchiveRes.h>

namespace sead {

void ArchiveRes::doCreate_(u8* buf, u32, Heap*)
{
    mEnable = prepareArchive_(buf);
}

ResourcePtr ArchiveRes::load(ResourceMgr::LoadArg& arg)
{
    ArchiveFileDevice dev(this);
    arg.device = &dev;
    return ResourceMgr::instance()->loadWithoutDecomp(arg);
}

} // namespace sead

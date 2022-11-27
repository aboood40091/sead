#include <mc/seadCoreInfo.h>

namespace sead {

u32 CoreInfo::sNumCores = 1;
u32 CoreInfo::sPlatformCoreId[32];
CoreId CoreInfo::sCoreIdFromPlatformCoreIdTable[32];

} // namespace sead

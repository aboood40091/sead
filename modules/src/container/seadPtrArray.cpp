#include <container/seadPtrArray.h>

namespace sead {

void PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buf)
{
    if (ptrNumMax > 0)
    {
        if (buf != NULL)
        {
            mPtrNumMax = ptrNumMax;
            mPtrs = static_cast<void**>(buf);
            mPtrNum = 0;
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "buf is null");
        }
    }
    else
    {
        //SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
    }
}

} // namespace sead

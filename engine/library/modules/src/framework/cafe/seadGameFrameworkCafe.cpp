#include <framework/cafe/seadGameFrameworkCafe.h>

namespace sead {

void GameFrameworkCafe::procDraw_()
{
    if (_6c.isOnBit(0) && !_6c.isOnBit(1))
        return;

    if (mDrawOrder == cDrawOrder_TV_DRC)
        drawTV_();
    else
        drawDRC_();

    if (!_36c)
    {
        if (mDrawOrder == cDrawOrder_TV_DRC)
            copyToTV_();
        else
            copyToDRC_();
    }

    if (mDrawOrder == cDrawOrder_TV_DRC)
        drawDRC_();
    else
        drawTV_();

    if (mDeferredCopyCallback == nullptr)
    {
        if (_36c)
        {
            if (mDrawOrder == cDrawOrder_TV_DRC)
                copyToTV_();
            else
                copyToDRC_();
        }

        if (mDrawOrder == cDrawOrder_TV_DRC)
            copyToDRC_();
        else
            copyToTV_();
    }

    GX2Flush();
}

void GameFrameworkCafe::swapBuffer_()
{
    if (_6c.isOnBit(0) && _6c.isOnBit(1))
        return;

    if (mDeferredCopyCallback != nullptr)
    {
        (*mDeferredCopyCallback)(0);
        {
            GX2DrawDone();

            if (_36c)
            {
                if (mDrawOrder == cDrawOrder_TV_DRC)
                    copyToTV_();
                else
                    copyToDRC_();
            }

            if (mDrawOrder == cDrawOrder_TV_DRC)
                copyToDRC_();
            else
                copyToTV_();

            GX2Flush();
        }
        (*mDeferredCopyCallback)(1);
    }

    if (mDisplayState == cDisplayState_Show)
        GX2SwapScanBuffers();

    GX2DrawDone();
}

} // namespace sead

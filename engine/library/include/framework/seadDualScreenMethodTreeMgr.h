#ifndef SEAD_DUALSCREEN_METHODTREEMGR_H_
#define SEAD_DUALSCREEN_METHODTREEMGR_H_

#include <framework/seadMethodTree.h>
#include <framework/seadMethodTreeMgr.h>

namespace sead {

class DualScreenMethodTreeMgr : public MethodTreeMgr
{
    SEAD_RTTI_OVERRIDE(DualScreenMethodTreeMgr, MethodTreeMgr)

public:
    enum Screen
    {
        cScreen_Top = 0,
        cScreen_Btm,
        cScreen_Num
    };

public:
    DualScreenMethodTreeMgr();
    virtual ~DualScreenMethodTreeMgr();

    virtual void attachMethod(s32, MethodTreeNode*);
    virtual MethodTreeNode* getRootMethodTreeNode(s32);
    virtual void pauseAll(bool);
    virtual void pauseAppCalc(bool);

    void calc();
    void drawTop();
    void drawBtm();

protected:
    MethodTreeNode mRootCalcNode;
    MethodTreeNode mSysCalcNode;
    MethodTreeNode mAppCalcNode;
    MethodTreeNode mTopRootDrawNode;
    MethodTreeNode mTopSysDrawNode;
    MethodTreeNode mTopAppDrawNode;
    MethodTreeNode mTopAppDrawFinalNode;
    MethodTreeNode mBtmRootDrawNode;
    MethodTreeNode mBtmSysDrawNode;
    MethodTreeNode mBtmAppDrawNode;
    MethodTreeNode mBtmAppDrawFinalNode;
    Screen mSysDrawScreen;
    Screen mAppDrawScreen;
};
#ifdef cafe
static_assert(sizeof(DualScreenMethodTreeMgr) == 0x3E4, "sead::DualScreenMethodTreeMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_DUALSCREEN_METHODTREEMGR_H_

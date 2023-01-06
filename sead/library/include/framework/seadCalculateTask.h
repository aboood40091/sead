#ifndef SEAD_CALCULATE_TASK_H_
#define SEAD_CALCULATE_TASK_H_

#include <framework/seadMethodTree.h>
#include <framework/seadTaskBase.h>

namespace sead {

class CalculateTask : public TaskBase
{
    SEAD_RTTI_OVERRIDE(CalculateTask, TaskBase)

public:
    CalculateTask(const TaskConstructArg& arg);
    CalculateTask(const TaskConstructArg& arg, const char* name);

    virtual void pauseCalc(bool b);
    virtual void pauseDraw(bool b);
    virtual void pauseCalcRec(bool b);
    virtual void pauseDrawRec(bool b);
    virtual void pauseCalcChild(bool b);
    virtual void pauseDrawChild(bool b);

    virtual ~CalculateTask();

    virtual void attachCalcImpl();
    virtual void attachDrawImpl();
    virtual void detachCalcImpl();
    virtual void detachDrawImpl();
    virtual const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const;
    virtual MethodTreeNode* getMethodTreeNode(s32 method_type);

    virtual void calc();

    MethodTreeNode mCalcNode;
};
#ifdef cafe
static_assert(sizeof(CalculateTask) == 0xC8, "sead::CalculateTask size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CALCULATE_TASK_H_

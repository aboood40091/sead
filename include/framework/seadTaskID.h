#ifndef SEAD_TASK_ID_H_
#define SEAD_TASK_ID_H_

#include <basis/seadTypes.h>

namespace sead {

class HeapArray;
class TaskBase;
class TaskMgr;
class TaskParameter;

struct TaskConstructArg
{
    inline TaskConstructArg();

    TaskConstructArg(HeapArray* heap_array_, TaskMgr* mgr_, TaskParameter* param_)
        : heap_array(heap_array_)
        , mgr(mgr_)
        , param(param_)
    {
    }

    HeapArray* heap_array;
    TaskMgr* mgr;
    TaskParameter* param;
};
#ifdef cafe
static_assert(sizeof(TaskConstructArg) == 0xC, "sead::TaskConstructArg size mismatch");
#endif // cafe

typedef TaskBase* (*TaskFactory)(const TaskConstructArg&);

class TaskClassID
{
public:
    enum Type
    {
        cInvalid = 0,
        cInt = 1,
        cFactory = 2,
        cString = 3
    };

public:
    Type mType;
    union
    {
        s32 mInt;
        TaskFactory mFactory;
        const char* mString;
    } mID;
};
#ifdef cafe
static_assert(sizeof(TaskClassID) == 8, "sead::TaskClassID size mismatch");
#endif // cafe

class TaskUserID
{
public:
    s32 mID;
};
#ifdef cafe
static_assert(sizeof(TaskUserID) == 4, "sead::TaskUserID size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_TASK_ID_H_

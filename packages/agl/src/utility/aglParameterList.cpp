#include <utility/aglParameter.h>
#include <utility/aglParameterList.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace utl {

IParameterList::IParameterList()
    : _70(0)
{
    mChildList.initOffset(offsetof(IParameterList, mListNode));
    mChildObj.initOffset(offsetof(IParameterObj, mListNode));

    setParameterListName_(sead::SafeString::cEmptyString);
}

void IParameterList::setParameterListName_(const sead::SafeString& name)
{
    mName = name;
    mNameHash = ParameterBase::calcHash(name);
}

void IParameterList::addObj(IParameterObj* child, const sead::SafeString& name)
{
    // SEAD_ASSERT(child != nullptr);
    child->mName = name;
    child->mNameHash = ParameterBase::calcHash(child->mName);

    mChildObj.pushBack(child);
}

} }

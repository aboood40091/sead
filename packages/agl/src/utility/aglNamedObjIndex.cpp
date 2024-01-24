#include <utility/aglNamedObjIndex.h>
#include <utility/aglNamedObjMgr.h>

namespace agl { namespace utl {

INamedObjIndex::INamedObjIndex()
    : mpObjMgr(nullptr)
    , mIndex(-1)
    , mpCallback(nullptr)
{
    setValue(sead::SafeString::cEmptyString);
}

INamedObjIndex::~INamedObjIndex()
{
    if (mListNode.isLinked())
        bind(nullptr);
}

void INamedObjIndex::bind(INamedObjMgr* p_mgr)
{
    if (p_mgr == mpObjMgr)
        return;

    if (mpObjMgr != nullptr)
        mpObjMgr->mObjIndex.erase(this);

    mpObjMgr = p_mgr;

    if (mpObjMgr != nullptr)
        mpObjMgr->mObjIndex.pushBack(this);
}

void INamedObjIndex::syncNameToIndex()
{
    if (mpObjMgr == nullptr)
        return;

    if (getValue() == sead::SafeString::cEmptyString)
    {
        mIndex = -1;
        setValue(sead::SafeString::cEmptyString); // ???
    }
    else
    {
        mIndex = -2;
        for (s32 i = 0, n = getNamedObjNum(); i < n; ++i)
        {
            if (getValue() == getNamedObjName(i))
            {
                mIndex = i;
                break;
            }
        }
    }

    if (mpCallback)
        mpCallback->callbackSyncNameToIndex(this);
}

} }

#ifndef SEAD_NAMABLE_H_
#define SEAD_NAMABLE_H_

#include <prim/seadSafeString.h>

namespace sead {

class INamable
{
public:
    INamable()
        : mINamableName()
    {
    }

    INamable(const SafeString& name)
        : mINamableName(name)
    {
    }

    void setName(const SafeString& name)
    {
        mINamableName = name;
    }

    const SafeString& getName() const
    {
        return mINamableName;
    }

private:
    SafeString mINamableName;
};

class IWNamable
{
public:
    IWNamable()
        : mIWNamableName()
    {
    }

    IWNamable(const WSafeString& name)
        : mIWNamableName(name)
    {
    }

    void setName(const WSafeString& name)
    {
        mIWNamableName = name;
    }

    const WSafeString& getName() const
    {
        return mIWNamableName;
    }

private:
    WSafeString mIWNamableName;
};

} // namespace sead

#endif // SEAD_NAMABLE_H_

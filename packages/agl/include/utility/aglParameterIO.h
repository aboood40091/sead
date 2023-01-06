#pragma once

#include <utility/aglParameterList.h>
#include <utility/aglResParameter.h>

namespace agl { namespace utl {

class IParameterIO : public IParameterList
{
public:
    IParameterIO();
    IParameterIO(const sead::SafeString& name, u32 = 0);
    virtual ~IParameterIO() { }

public:
    virtual bool save(const sead::SafeString&, u32) const;
    virtual void applyResParameterArchive(ResParameterArchive arc);
    virtual void applyResParameterArchiveLerp(ResParameterArchive arc_a, ResParameterArchive arc_b, f32 t);

protected:
    virtual void callbackInvalidVersion_(ResParameterArchive arc) { }

public:
    void setDelegate(void* delegate)
    {
        mpDelegate = delegate;
    }

protected:
    sead::FixedSafeString<64> _80;
    u32 _cc;
    void* mpDelegate; // sead delegate
    sead::FixedSafeString<256> _d4;
};
static_assert(sizeof(IParameterIO) == 0x1E0, "agl::utl::IParameterIO size mismatch");

} }

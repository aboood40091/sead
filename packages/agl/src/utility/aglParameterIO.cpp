#include <gfx/seadGraphics.h>
#include <utility/aglParameterIO.h>

namespace agl { namespace utl {

IParameterIO::IParameterIO(const sead::SafeString& param_1, u32 param_2)
    : IParameterList()
    , mpDelegate(nullptr)
    , _d4(sead::SafeString::cEmptyString)
{
    _80 = param_1;
    _cc = param_2;
    setParameterListName_("param_root");
}

bool IParameterIO::save(const sead::SafeString&, u32) const
{
    // Deleted from NSMBU
    // Uses sead::XmlDocument to write this parameter to an XML file
    return false;
}

void IParameterIO::applyResParameterArchive(ResParameterArchive arc)
{
    sead::Graphics::instance()->lockDrawContext();
    {
        if (_cc != arc.ref()._10)
            callbackInvalidVersion_(arc);

        applyResParameterList(arc.getResParameterList());
    }
    sead::Graphics::instance()->unlockDrawContext();
}

void IParameterIO::applyResParameterArchiveLerp(ResParameterArchive arc_a, ResParameterArchive arc_b, f32 t)
{
    // Deleted from NSMBU
    // Implementation seems to be different from newer games
}

} }

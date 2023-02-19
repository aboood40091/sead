#include <gfx/seadGraphics.h>
#include <utility/aglParameterIO.h>

namespace agl { namespace utl {

IParameterIO::IParameterIO(const sead::SafeString& type, u32 version)
    : IParameterList()
    , mpDelegate(nullptr)
    , _d4(sead::SafeString::cEmptyString)
{
    mType = type;
    mVersion = version;
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
        // SEAD_ASSERT(arc.isValid());

        if (mVersion != arc.ref().mTypeVersion)
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

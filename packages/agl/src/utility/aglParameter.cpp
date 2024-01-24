#include <codec/seadHashCRC32.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace utl {

ParameterBase::ParameterBase()
{
    initializeListNode("default", "parameter", "", nullptr);
}

ParameterBase::ParameterBase(const sead::SafeString& name, const sead::SafeString& label, IParameterObj* p_obj)
{
    initializeListNode(name, label, "", p_obj);
}

ParameterBase::ParameterBase(const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj)
{
    initializeListNode(name, label, meta, p_obj);
}

bool ParameterBase::makeZero()
{
    bool ret = false;
    switch (getParameterType())
    {
    case cType_f32:
        *static_cast<f32*>(ptr()) = 0.0f;
        ret = true;
        break;
    case cType_vec2:
        static_cast<sead::Vector2f*>(ptr())->set(0.0f, 0.0f);
        ret = true;
        break;
    case cType_vec3:
        static_cast<sead::Vector3f*>(ptr())->set(0.0f, 0.0f, 0.0f);
        ret = true;
        break;
    case cType_vec4:
        static_cast<sead::Vector4f*>(ptr())->set(0.0f, 0.0f, 0.0f, 0.0f);
        ret = true;
        break;
    case cType_color:
        static_cast<sead::Color4f*>(ptr())->setf(0.0f, 0.0f, 0.0f, 0.0f);
        ret = true;
        break;
    default:
        break;
    }
    return ret;
}

void ParameterBase::applyResource(ResParameter res)
{
    if (getParameterType() != cType_bool)
        sead::MemUtil::copy(ptr(), res.getValue(), size());
    else
        *static_cast<bool*>(ptr()) = *static_cast<const u8*>(res.getValue()) != 0; // Literally only this line doesn't match

    postApplyResource_(res.getValue(), res.ptr()->mSize - sizeof(ResParameterData));
}

void ParameterBase::applyResource(ResParameter res, f32 t)
{
    switch (getParameterType())
    {
    case cType_bool:
        *static_cast<bool*>(ptr()) = *static_cast<const u8*>(res.getValue()) != 0; // Same issue as function above
        break;
    case cType_f32:
        {
            f32 temp;
            sead::MemUtil::copy(&temp, res.getValue(), size());
            *static_cast<f32*>(ptr()) += temp * t;
        }
        break;
    case cType_vec2:
        {
            sead::Vector2f temp;
            sead::MemUtil::copy(&temp, res.getValue(), size());
            *static_cast<sead::Vector2f*>(ptr()) += temp * t;
        }
        break;
    case cType_vec3:
        {
            sead::Vector3f temp;
            sead::MemUtil::copy(&temp, res.getValue(), size());
            *static_cast<sead::Vector3f*>(ptr()) += temp * t;
        }
        break;
    case cType_vec4:
        {
            sead::Vector4f temp;
            sead::MemUtil::copy(&temp, res.getValue(), size());
            *static_cast<sead::Vector4f*>(ptr()) += temp * t;
        }
        break;
    case cType_color:
        {
            sead::Color4f temp;
            sead::MemUtil::copy(&temp, res.getValue(), size());
            *static_cast<sead::Color4f*>(ptr()) += temp * sead::Color4f(t, t, t, t);
        }
        break;
    case cType_int:
    case cType_string32:
    case cType_string64:
        sead::MemUtil::copy(ptr(), res.getValue(), size());
        break;
    case cType_curve1:
    case cType_curve2:
    case cType_curve3:
    case cType_curve4:
        break;
    default:
        // SEAD_ASSERT_MSG(false, "%d", s32(getParameterType()));
    }

    postApplyResource_(res.getValue(), res.ptr()->mSize - sizeof(ResParameterData));
}

bool ParameterBase::copy(const ParameterBase& src)
{
    if (getParameterType() != src.getParameterType() ||
        mHash != src.mHash)
    {
        return false;
    }

    copyUnsafe(src);
    return true;
}

void ParameterBase::copyUnsafe(const ParameterBase& src)
{
    u8* p_dst = static_cast<u8*>(ptr());
    const u8* p_src = static_cast<const u8*>(src.ptr());
    s32 len = size();

    for (s32 i = 0; i < len; i++)
        *p_dst++ = *p_src++;
}

bool ParameterBase::copyLerp(const ParameterBase& src_a, const ParameterBase& src_b, f32 t)
{
    if (getParameterType() != src_a.getParameterType() ||
        mHash != src_a.mHash ||
        getParameterType() != src_b.getParameterType() ||
        mHash != src_b.mHash)
    {
        return false;
    }

    switch (getParameterType())
    {
    case cType_bool:
    case cType_int:
    case cType_string32:
    case cType_string64:
        copyUnsafe(src_a);
        break;
    case cType_f32:
        copyLerp_<f32>(src_a, src_b, t);
        break;
    case cType_vec2:
        copyLerp_<sead::Vector2f>(src_a, src_b, t);
        break;
    case cType_vec3:
        copyLerp_<sead::Vector3f>(src_a, src_b, t);
        break;
    case cType_vec4:
        copyLerp_<sead::Vector4f>(src_a, src_b, t);
        break;
    case cType_color:
        copyLerp_<sead::Color4f>(src_a, src_b, t);
        break;
    case cType_curve1:
    case cType_curve2:
    case cType_curve3:
    case cType_curve4:
        break;
    default:
        // SEAD_ASSERT_MSG(false, "%d", s32(getParameterType()));
    }
    return true;
}

void ParameterBase::writeToXML(sead::XmlElement*, sead::Heap*) const
{
    // Deleted from NSMBU
}

s32 ParameterBase::readFromXML(const sead::XmlElement&)
{
    // Deleted from NSMBU
    return 2;
}

void ParameterBase::initializeListNode(const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj)
{
    mHash = calcHash(name);

    if (p_obj)
        p_obj->pushBackListNode(this);
}

u32 ParameterBase::calcHash(const sead::SafeString& s)
{
    return sead::HashCRC32::calcHash(s.cstr(), s.calcLength());
}

} }

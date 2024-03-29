#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <prim/seadNamable.h>

namespace agl {

class ShaderLocation
{
public:
    ShaderLocation()
        : mVS(-1)
        , mFS(-1)
        , mGS(-1)
    {
    }

    bool isValid() const
    {
        return getVertexLocation()   != -1 ||
               getFragmentLocation() != -1 ||
               getGeometryLocation() != -1;
    }

    s32 getVertexLocation() const { return mVS; }
    s32 getFragmentLocation() const { return mFS; }
    s32 getGeometryLocation() const { return mGS; }

protected:
    s16 mVS;
    s16 mFS;
    s16 mGS;
};
static_assert(sizeof(ShaderLocation) == 6, "agl::ShaderLocation size mismatch");

class ShaderProgram;

class UniformLocation : public sead::INamable, public ShaderLocation
{
public:
    UniformLocation()
        : sead::INamable("Undefined")
    {
    }

    explicit UniformLocation(const sead::SafeString& name)
        : sead::INamable(name)
    {
    }

    UniformLocation(const sead::SafeString& name, const ShaderProgram& program)
        : sead::INamable(name)
    {
        search(program);
    }

    UniformLocation& operator=(const UniformLocation& rhs)
    {
        setName(rhs.getName());
        ShaderLocation::operator=(static_cast<const ShaderLocation&>(rhs));
        return *this;
    }

    void search(const ShaderProgram& program);

public:
    void setBool(bool value) const;

    void setInt(s32 value) const;
    void setUInt(u32 value) const;
    void setFloat(f32 value) const;

    void setIVec2(const sead::Vector2i& value) const;
    void setUVec2(const sead::Vector2u& value) const;
    void setVec2(const sead::Vector2f& value) const;

    void setIVec3(const sead::Vector3i& value) const;
    void setUVec3(const sead::Vector3u& value) const;
    void setVec3(const sead::Vector3f& value) const;

    void setIVec4(const sead::Vector4i& value) const;
    void setUVec4(const sead::Vector4u& value) const;
    void setVec4(const sead::Vector4f& value) const;

    void setVec4(const sead::Color4f& value) const;

    void setVec4Array(const sead::Matrix34f& value) const;  // vec4[3]
    void setVec4Array(const sead::Matrix44f& value) const;  // vec4[4]

    void setMtx43(const f32* value) const;
    void setMtx44(const f32* value) const;
};
static_assert(sizeof(UniformLocation) == 0x10, "agl::UniformLocation size mismatch");

class UniformBlockLocation : public sead::INamable, public ShaderLocation
{
public:
    UniformBlockLocation()
        : sead::INamable("Undefined")
    {
    }

    explicit UniformBlockLocation(const sead::SafeString& name)
        : sead::INamable(name)
    {
    }

    UniformBlockLocation(const sead::SafeString& name, const ShaderProgram& program)
        : sead::INamable(name)
    {
        search(program);
    }

    UniformBlockLocation& operator=(const UniformBlockLocation& rhs)
    {
        setName(rhs.getName());
        ShaderLocation::operator=(static_cast<const ShaderLocation&>(rhs));
        return *this;
    }

    void search(const ShaderProgram& program);
};
static_assert(sizeof(UniformBlockLocation) == 0x10, "agl::UniformBlockLocation size mismatch");

class SamplerLocation : public sead::INamable, public ShaderLocation
{
public:
    SamplerLocation()
        : sead::INamable("Undefined")
    {
    }

    explicit SamplerLocation(const sead::SafeString& name)
        : sead::INamable(name)
    {
    }

    SamplerLocation(const sead::SafeString& name, const ShaderProgram& program)
        : sead::INamable(name)
    {
        search(program);
    }

    SamplerLocation& operator=(const SamplerLocation& rhs)
    {
        setName(rhs.getName());
        ShaderLocation::operator=(static_cast<const ShaderLocation&>(rhs));
        return *this;
    }

    void search(const ShaderProgram& program);
};
static_assert(sizeof(SamplerLocation) == 0x10, "agl::SamplerLocation size mismatch");

class AttributeLocation : public sead::INamable
{
public:
    AttributeLocation()
        : sead::INamable("Undefined")
        , mVS() // Nintendo mistakingly didn't set this to -1
    {
    }

    explicit AttributeLocation(const sead::SafeString& name)
        : sead::INamable(name)
        , mVS() // I don't actually know if this is set here
    {
    }

    AttributeLocation(const sead::SafeString& name, const ShaderProgram& program)
        : sead::INamable(name)
      //, mVS() // Set by search()
    {
        search(program);
    }

    bool isValid() const
    {
        return getVertexLocation() != -1;
    }

    s32 getVertexLocation() const { return mVS; }

    void search(const ShaderProgram& program);

private:
    s16 mVS;
};
static_assert(sizeof(AttributeLocation) == 0xC, "agl::AttributeLocation size mismatch");

}

#ifdef __cplusplus

#include <common/aglShaderLocation.hpp>

#endif // __cplusplus

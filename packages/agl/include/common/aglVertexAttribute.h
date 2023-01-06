#pragma once

#include <container/seadBuffer.h>
#include <container/seadSafeArray.h>

#include <cafe/gx2.h>

namespace agl {

class VertexBuffer;

class VertexAttribute
{
    struct Attribute_
    {
        Attribute_()
            : mpVertexBuffer(nullptr)
            , mStreamIndex(-1)
            , mBufferIndex(-1)
        {
        }

        const VertexBuffer* mpVertexBuffer;
        s32 mStreamIndex;
        s32 mBufferIndex;
    };
    static_assert(sizeof(Attribute_) == 0xC, "agl::VertexAttribute::Attribute_ size mismatch");

public:
    static const u32 cVertexAttributeMax = 16;

public:
    VertexAttribute();
    virtual ~VertexAttribute();

    void create(u32 buffer_max, sead::Heap* heap = nullptr);
    void cleanUp();
    void destroy();

    void setVertexStream(s16 location, const VertexBuffer* buffer, u32 stream_index);
    void setUp();

    void activate() const;

private:
    s32 enableVertexBuffer_(Attribute_* attr, const VertexBuffer* buffer, u32 stream_index);
    s32 disableVertexBuffer_(Attribute_* attr);

private:
    sead::SafeArray<Attribute_, cVertexAttributeMax> mAttribute;
    sead::Buffer<const VertexBuffer*> mVertexBuffer;
    bool mSetupFinish;
    bool mCreateFinish;
    GX2FetchShader mFetchShader;
    u8* mpFetchShaderBuf;
};
static_assert(sizeof(VertexAttribute) == 0xF4, "agl::VertexAttribute size mismatch");

}

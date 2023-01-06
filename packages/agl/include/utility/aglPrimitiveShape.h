#pragma once

#include <common/aglVertexBuffer.h>
#include <common/aglIndexStream.h>
#include <container/seadBuffer.h>
#include <container/seadSafeArray.h>
#include <heap/seadDisposer.h>
#include <math/seadVector.h>

namespace agl { namespace utl {

class PrimitiveShape
{
    SEAD_SINGLETON_DISPOSER(PrimitiveShape)

public:
    enum DrawType
    {
        cDrawType_Triangle = 0,
        cDrawType_Line,
        cDrawType_Point,
        cDrawType_Num
    };
    static_assert(cDrawType_Num == 3);

    enum Quality
    {
        cQuality_0 = 0,
        cQuality_1,
        cQuality_2,
        cQuality_Num
    };
    static_assert(cQuality_Num == 3);

    struct Vertex
    {
        sead::Vector3f pos;
        sead::Vector3f nrm;
        sead::Vector2f tex;
    };
    static_assert(sizeof(Vertex) == 0x20, "agl::utl::PrimitiveShape::Vertex size mismatch");

public:
    PrimitiveShape();
    ~PrimitiveShape();

    const IndexStream& getIdxStreamQuadTriangle(DrawType draw_type = cDrawType_Triangle) const
    {
        if (draw_type == cDrawType_Triangle)
            return mIdxStreamQuadTriangle;

        else // if (draw_type == cDrawType_Line)
            return mIdxStreamLineQuadTriangle;
    }

private:
    // Quad
    sead::Buffer<Vertex>    mVtxQuad;
    sead::Buffer<u32>       mIdxQuad;
    sead::Buffer<u32>       mIdxLineQuad;
    VertexBuffer            mVtxBufferQuad;
    IndexStream             mIdxStreamQuad;
    IndexStream             mIdxStreamLineQuad;

    // QuadTriangle
    sead::Buffer<Vertex>    mVtxQuadTriangle;
    sead::Buffer<u32>       mIdxQuadTriangle;
    sead::Buffer<u32>       mIdxLineQuadTriangle;
    VertexBuffer            mVtxBufferQuadTriangle;
    IndexStream             mIdxStreamQuadTriangle;
    IndexStream             mIdxStreamLineQuadTriangle;

    // Cube
    sead::Buffer<Vertex>    mVtxCube;
    sead::Buffer<u32>       mIdxCube;
    sead::Buffer<u32>       mIdxLineCube;
    VertexBuffer            mVtxBufferCube;
    IndexStream             mIdxStreamCube;
    IndexStream             mIdxStreamLineCube;

    // Circle
    sead::Buffer<Vertex>                                                                    mVtxCircle;
    sead::UnsafeArray<sead::UnsafeArray<sead::Buffer<u16>,  cDrawType_Num>, cQuality_Num>   mIdxCircle;
    VertexBuffer                                                                            mVtxBufferCircle;
    sead::UnsafeArray<sead::UnsafeArray<IndexStream,        cDrawType_Num>, cQuality_Num>   mIdxStreamCircle;

    // Sphere
    sead::Buffer<Vertex>                                                                    mVtxSphere;
    sead::UnsafeArray<sead::UnsafeArray<sead::Buffer<u16>,  cDrawType_Num>, cQuality_Num>   mIdxSphere;
    VertexBuffer                                                                            mVtxBufferSphere;
    sead::UnsafeArray<sead::UnsafeArray<IndexStream,        cDrawType_Num>, cQuality_Num>   mIdxStreamSphere;

    // Hemisphere
    sead::Buffer<Vertex>                                                                    mVtxHemisphere;
    sead::UnsafeArray<sead::UnsafeArray<sead::Buffer<u16>,  cDrawType_Num>, cQuality_Num>   mIdxHemisphere;
    VertexBuffer                                                                            mVtxBufferHemisphere;
    sead::UnsafeArray<sead::UnsafeArray<IndexStream,        cDrawType_Num>, cQuality_Num>   mIdxStreamHemisphere;

    // Cylinder
    sead::Buffer<Vertex>                                                                    mVtxCylinder;
    sead::UnsafeArray<sead::Buffer<u32>,                                    cQuality_Num>   mIdxCylinder;
    sead::UnsafeArray<sead::Buffer<u32>,                                    cQuality_Num>   mIdxLineCylinder;
    VertexBuffer                                                                            mVtxBufferCylinder;
    sead::UnsafeArray<IndexStream,                                          cQuality_Num>   mIdxStreamCylinder;
    sead::UnsafeArray<IndexStream,                                          cQuality_Num>   mIdxStreamLineCylinder;

    // Cone
    sead::Buffer<Vertex>                                                                    mVtxCone;
    sead::UnsafeArray<sead::Buffer<u32>,                                    cQuality_Num>   mIdxCone;
    sead::UnsafeArray<sead::Buffer<u32>,                                    cQuality_Num>   mIdxLineCone;
    VertexBuffer                                                                            mVtxBufferCone;
    sead::UnsafeArray<IndexStream,                                          cQuality_Num>   mIdxStreamCone;
    sead::UnsafeArray<IndexStream,                                          cQuality_Num>   mIdxStreamLineCone;

    // Torus
    sead::Buffer<Vertex>                                                                    mVtxTorus;
    sead::UnsafeArray<sead::Buffer<u32>,                                    cQuality_Num>   mIdxTorus;
    sead::UnsafeArray<sead::Buffer<u32>,                                    cQuality_Num>   mIdxLineTorus;
    VertexBuffer                                                                            mVtxBufferTorus;
    sead::UnsafeArray<IndexStream,                                          cQuality_Num>   mIdxStreamTorus;
    sead::UnsafeArray<IndexStream,                                          cQuality_Num>   mIdxStreamLineTorus;
};
static_assert(sizeof(PrimitiveShape) == 0x172C, "agl::utl::PrimitiveShape size mismatch");

} }

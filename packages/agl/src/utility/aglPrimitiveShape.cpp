#include <utility/aglPrimitiveShape.h>

namespace agl { namespace utl {

SEAD_SINGLETON_DISPOSER_IMPL(PrimitiveShape)

PrimitiveShape::PrimitiveShape()
{
}

PrimitiveShape::~PrimitiveShape()
{
    mVtxQuad.freeBuffer();
    mVtxQuadTriangle.freeBuffer();
    mVtxCube.freeBuffer();
    mVtxSphere.freeBuffer();
    mVtxHemisphere.freeBuffer();
    mVtxCylinder.freeBuffer();
    mVtxCone.freeBuffer();
    mVtxTorus.freeBuffer();
    mVtxCircle.freeBuffer();
    mIdxQuad.freeBuffer();
    mIdxLineQuad.freeBuffer();
    mIdxQuadTriangle.freeBuffer();
    mIdxLineQuadTriangle.freeBuffer();
    mIdxCube.freeBuffer();
    mIdxLineCube.freeBuffer();

    for (s32 i = 0; i < cQuality_Num; i++)
    {
        mIdxCylinder[i].freeBuffer();
        mIdxLineCylinder[i].freeBuffer();
        mIdxCone[i].freeBuffer();
        mIdxLineCone[i].freeBuffer();
        mIdxTorus[i].freeBuffer();
        mIdxLineTorus[i].freeBuffer();
    }

    for (s32 i = 0; i < cQuality_Num; i++)
    {
        for (s32 j = 0; j < cDrawType_Num; j++)
        {
            mIdxSphere[i][j].freeBuffer();
            mIdxHemisphere[i][j].freeBuffer();
            mIdxCircle[i][j].freeBuffer();
        }
    }
}

void PrimitiveShape::initialize(sead::Heap* heap)
{
  setUpStreamQuad_(heap);
  setUpStreamQuadTriangle_(heap);
  setUpStreamCube_(heap);
  setUpStreamCircle_(32, heap);
  setUpStreamSphere_(32, 16, heap);
  setUpStreamHemisphere_(32, 16, heap);
  setUpStreamCylinder_(32, 16, heap);
  setUpStreamCone_(32, 16, heap);
  setUpStreamTorus_(32, 32, heap, 1 / 3.f, 1 / 6.f, 0, 1);
}

void PrimitiveShape::setUpStreamQuad_(sead::Heap* heap)
{
    // SEAD_ASSERT(mIdxStreamQuad.getCount() == 0);

    const u32 cVtxNum = 4;
    const u32 cIdxNum = 6;
    const u32 cIdxLineNum = 4;

    mVtxQuad.allocBuffer(cVtxNum * sizeof(Vertex), heap);
    {
        sead::UnsafeArray<Vertex, cVtxNum> vertices;

        vertices[0].pos.x = -0.5f;
        vertices[0].pos.y =  0.5f;
        vertices[0].pos.z =  0.0f;
        vertices[0].nrm.x =  0.0f;
        vertices[0].nrm.y =  0.0f;
        vertices[0].nrm.z =  1.0f;
        vertices[0].tex.x =  0.0f;
        vertices[0].tex.y =  0.0f;

        vertices[1].pos.x =  0.5f;
        vertices[1].pos.y =  0.5f;
        vertices[1].pos.z =  0.0f;
        vertices[1].nrm.x =  0.0f;
        vertices[1].nrm.y =  0.0f;
        vertices[1].nrm.z =  1.0f;
        vertices[1].tex.x =  1.0f;
        vertices[1].tex.y =  0.0f;

        vertices[2].pos.x = -0.5f;
        vertices[2].pos.y = -0.5f;
        vertices[2].pos.z =  0.0f;
        vertices[2].nrm.x =  0.0f;
        vertices[2].nrm.y =  0.0f;
        vertices[2].nrm.z =  1.0f;
        vertices[2].tex.x =  0.0f;
        vertices[2].tex.y =  1.0f;

        vertices[3].pos.x =  0.5f;
        vertices[3].pos.y = -0.5f;
        vertices[3].pos.z =  0.0f;
        vertices[3].nrm.x =  0.0f;
        vertices[3].nrm.y =  0.0f;
        vertices[3].nrm.z =  1.0f;
        vertices[3].tex.x =  1.0f;
        vertices[3].tex.y =  1.0f;

        for (u32 i = 0; i < cVtxNum; i++)
        {
            mVtxQuad[i].pos = vertices[i].pos;
            mVtxQuad[i].nrm = vertices[i].nrm;
            mVtxQuad[i].tex = vertices[i].tex;
        }
    }
    mVtxBufferQuad.setUpBuffer(mVtxQuad.getBufferPtr(), sizeof(Vertex), cVtxNum * sizeof(Vertex));
    setUpStreams_(&mVtxBufferQuad);

    mIdxQuad.allocBuffer(cIdxNum * sizeof(u32), heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxNum> indices;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        for (u32 i = 0; i < cVtxNum; i++)
            mIdxQuad[i] = indices[i];
    }
    mIdxStreamQuad.setUpStream(mIdxQuad.getBufferPtr(), cIdxNum);

    mIdxLineQuad.allocBuffer(cIdxLineNum * sizeof(u32), heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxLineNum> indices;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 3;
        indices[3] = 2;

        for (u32 i = 0; i < cVtxNum; i++)
            mIdxLineQuad[i] = indices[i];
    }
    mIdxStreamLineQuad.setUpStream(mIdxLineQuad.getBufferPtr(), cIdxLineNum, GX2_PRIMITIVE_LINE_LOOP);
}

void PrimitiveShape::setUpStreamQuadTriangle_(sead::Heap* heap)
{
    // SEAD_ASSERT(mIdxStreamQuadTriangle.getCount() == 0);

    const u32 cVtxNum = 3;
    const u32 cIdxNum = 3;
    const u32 cIdxLineNum = 3;

    mVtxQuadTriangle.allocBuffer(cVtxNum * sizeof(Vertex), heap);
    {
        sead::UnsafeArray<Vertex, cVtxNum> vertices;

        vertices[0].pos.x = -0.5f;
        vertices[0].pos.y =  0.5f;
        vertices[0].pos.z =  0.0f;
        vertices[0].nrm.x =  0.0f;
        vertices[0].nrm.y =  0.0f;
        vertices[0].nrm.z =  1.0f;
        vertices[0].tex.x =  0.0f;
        vertices[0].tex.y =  0.0f;

        vertices[1].pos.x =  1.5f;
        vertices[1].pos.y =  0.5f;
        vertices[1].pos.z =  0.0f;
        vertices[1].nrm.x =  0.0f;
        vertices[1].nrm.y =  0.0f;
        vertices[1].nrm.z =  1.0f;
        vertices[1].tex.x =  2.0f;
        vertices[1].tex.y =  0.0f;

        vertices[2].pos.x = -0.5f;
        vertices[2].pos.y = -1.5f;
        vertices[2].pos.z =  0.0f;
        vertices[2].nrm.x =  0.0f;
        vertices[2].nrm.y =  0.0f;
        vertices[2].nrm.z =  1.0f;
        vertices[2].tex.x =  0.0f;
        vertices[2].tex.y =  2.0f;

        for (u32 i = 0; i < cVtxNum; i++)
        {
            mVtxQuadTriangle[i].pos = vertices[i].pos;
            mVtxQuadTriangle[i].nrm = vertices[i].nrm;
            mVtxQuadTriangle[i].tex = vertices[i].tex;
        }
    }
    mVtxBufferQuadTriangle.setUpBuffer(mVtxQuadTriangle.getBufferPtr(), sizeof(Vertex), cVtxNum * sizeof(Vertex));
    setUpStreams_(&mVtxBufferQuadTriangle);

    mIdxQuadTriangle.allocBuffer(cIdxNum * sizeof(u32), heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxNum> indices;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;

        for (u32 i = 0; i < cVtxNum; i++)
            mIdxQuadTriangle[i] = indices[i];
    }
    mIdxStreamQuadTriangle.setUpStream(mIdxQuadTriangle.getBufferPtr(), cIdxNum);

    mIdxLineQuadTriangle.allocBuffer(cIdxLineNum * sizeof(u32), heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxLineNum> indices;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;

        for (u32 i = 0; i < cVtxNum; i++)
            mIdxLineQuadTriangle[i] = indices[i];
    }
    mIdxStreamLineQuadTriangle.setUpStream(mIdxLineQuadTriangle.getBufferPtr(), cIdxLineNum, GX2_PRIMITIVE_LINE_LOOP);
}

/*
void PrimitiveShape::setUpStreamCube_(sead::Heap* heap);
void PrimitiveShape::setUpStreamCircle_(u32, sead::Heap* heap);
void PrimitiveShape::setUpStreamSphere_(u32, u32, sead::Heap* heap);
void PrimitiveShape::setUpStreamHemisphere_(u32, u32, sead::Heap* heap);
void PrimitiveShape::setUpStreamCylinder_(u32, u32, sead::Heap* heap);
void PrimitiveShape::setUpStreamCone_(u32, u32, sead::Heap* heap);
void PrimitiveShape::setUpStreamTorus_(u32, u32, sead::Heap* heap, f32, f32, s32, s32);
*/

void PrimitiveShape::setUpStreams_(VertexBuffer* p_vtx_buffer)
{
    p_vtx_buffer->setUpStream(0, cVertexStreamFormat_32_32_32_float,    offsetof(Vertex, pos));
    p_vtx_buffer->setUpStream(1, cVertexStreamFormat_32_32_32_float,    offsetof(Vertex, nrm));
    p_vtx_buffer->setUpStream(2, cVertexStreamFormat_32_32_float,       offsetof(Vertex, tex));
}

} }


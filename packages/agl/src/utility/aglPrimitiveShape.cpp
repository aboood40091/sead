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

    mVtxQuad.allocBuffer(cVtxNum, heap);
    {
        sead::UnsafeArray<Vertex, cVtxNum> vertices;

        vertices[0].pos[0] = -0.5f;
        vertices[0].pos[1] =  0.5f;
        vertices[0].pos[2] =  0.0f;
        vertices[0].nrm[0] =  0.0f;
        vertices[0].nrm[1] =  0.0f;
        vertices[0].nrm[2] =  1.0f;
        vertices[0].tex[0] =  0.0f;
        vertices[0].tex[1] =  0.0f;

        vertices[1].pos[0] =  0.5f;
        vertices[1].pos[1] =  0.5f;
        vertices[1].pos[2] =  0.0f;
        vertices[1].nrm[0] =  0.0f;
        vertices[1].nrm[1] =  0.0f;
        vertices[1].nrm[2] =  1.0f;
        vertices[1].tex[0] =  1.0f;
        vertices[1].tex[1] =  0.0f;

        vertices[2].pos[0] = -0.5f;
        vertices[2].pos[1] = -0.5f;
        vertices[2].pos[2] =  0.0f;
        vertices[2].nrm[0] =  0.0f;
        vertices[2].nrm[1] =  0.0f;
        vertices[2].nrm[2] =  1.0f;
        vertices[2].tex[0] =  0.0f;
        vertices[2].tex[1] =  1.0f;

        vertices[3].pos[0] =  0.5f;
        vertices[3].pos[1] = -0.5f;
        vertices[3].pos[2] =  0.0f;
        vertices[3].nrm[0] =  0.0f;
        vertices[3].nrm[1] =  0.0f;
        vertices[3].nrm[2] =  1.0f;
        vertices[3].tex[0] =  1.0f;
        vertices[3].tex[1] =  1.0f;

        for (u32 i = 0; i < cVtxNum; i++)
        {
            *(sead::Vector3f*)mVtxQuad[i].pos = *(sead::Vector3f*)vertices[i].pos;
            *(sead::Vector3f*)mVtxQuad[i].nrm = *(sead::Vector3f*)vertices[i].nrm;
            *(sead::Vector2f*)mVtxQuad[i].tex = *(sead::Vector2f*)vertices[i].tex;
        }
    }
    mVtxBufferQuad.setUpBuffer(mVtxQuad.getBufferPtr(), sizeof(Vertex), cVtxNum * sizeof(Vertex));
    setUpStreams_(&mVtxBufferQuad);

    mIdxQuad.allocBuffer(cIdxNum, heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxNum> indices;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        for (u32 i = 0; i < cIdxNum; i++)
            mIdxQuad[i] = indices[i];
    }
    mIdxStreamQuad.setUpStream(mIdxQuad.getBufferPtr(), cIdxNum);

    mIdxLineQuad.allocBuffer(cIdxLineNum, heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxLineNum> indices;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 3;
        indices[3] = 2;

        for (u32 i = 0; i < cIdxLineNum; i++)
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

    mVtxQuadTriangle.allocBuffer(cVtxNum, heap);
    {
        sead::UnsafeArray<Vertex, cVtxNum> vertices;

        vertices[0].pos[0] = -0.5f;
        vertices[0].pos[1] =  0.5f;
        vertices[0].pos[2] =  0.0f;
        vertices[0].nrm[0] =  0.0f;
        vertices[0].nrm[1] =  0.0f;
        vertices[0].nrm[2] =  1.0f;
        vertices[0].tex[0] =  0.0f;
        vertices[0].tex[1] =  0.0f;

        vertices[1].pos[0] =  1.5f;
        vertices[1].pos[1] =  0.5f;
        vertices[1].pos[2] =  0.0f;
        vertices[1].nrm[0] =  0.0f;
        vertices[1].nrm[1] =  0.0f;
        vertices[1].nrm[2] =  1.0f;
        vertices[1].tex[0] =  2.0f;
        vertices[1].tex[1] =  0.0f;

        vertices[2].pos[0] = -0.5f;
        vertices[2].pos[1] = -1.5f;
        vertices[2].pos[2] =  0.0f;
        vertices[2].nrm[0] =  0.0f;
        vertices[2].nrm[1] =  0.0f;
        vertices[2].nrm[2] =  1.0f;
        vertices[2].tex[0] =  0.0f;
        vertices[2].tex[1] =  2.0f;

        for (u32 i = 0; i < cVtxNum; i++)
        {
            *(sead::Vector3f*)mVtxQuadTriangle[i].pos = *(sead::Vector3f*)vertices[i].pos;
            *(sead::Vector3f*)mVtxQuadTriangle[i].nrm = *(sead::Vector3f*)vertices[i].nrm;
            *(sead::Vector2f*)mVtxQuadTriangle[i].tex = *(sead::Vector2f*)vertices[i].tex;
        }
    }
    mVtxBufferQuadTriangle.setUpBuffer(mVtxQuadTriangle.getBufferPtr(), sizeof(Vertex), cVtxNum * sizeof(Vertex));
    setUpStreams_(&mVtxBufferQuadTriangle);

    mIdxQuadTriangle.allocBuffer(cIdxNum, heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxNum> indices;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;

        for (u32 i = 0; i < cIdxNum; i++)
            mIdxQuadTriangle[i] = indices[i];
    }
    mIdxStreamQuadTriangle.setUpStream(mIdxQuadTriangle.getBufferPtr(), cIdxNum);

    mIdxLineQuadTriangle.allocBuffer(cIdxLineNum, heap, GX2_INDEX_BUFFER_ALIGNMENT);
    {
        sead::UnsafeArray<u32, cIdxLineNum> indices;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;

        for (u32 i = 0; i < cIdxLineNum; i++)
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


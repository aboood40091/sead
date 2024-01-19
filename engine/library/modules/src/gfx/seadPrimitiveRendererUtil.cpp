#include <gfx/seadPrimitiveRendererUtil.h>
#include <math/seadMathCalcCommon.h>
#include <prim/seadMemUtil.h>

namespace sead { namespace PrimitiveRendererUtil {

void setQuadVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[4] = {
        { Vector3f(-0.5f,  0.5f, 0.0f), Vector2f(0.0f, 1.0f), Color4f(0.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f,  0.5f, 0.0f), Vector2f(1.0f, 1.0f), Color4f(0.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f(-0.5f, -0.5f, 0.0f), Vector2f(0.0f, 0.0f), Color4f(1.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f, -0.5f, 0.0f), Vector2f(1.0f, 0.0f), Color4f(1.0f, 0.0f, 0.0f, 0.0f) }
    };

    static const u16 cIdx[6] = {
        0, 2, 1,
        1, 2, 3
    };

    if (vtx != nullptr)
        MemUtil::copy(vtx, cVtx, sizeof(cVtx));

    if (idx != nullptr)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void setLineVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[2] = {
        { Vector3f(-0.5f,  0.0f, 0.0f), Vector2f(0.0f, 0.5f), Color4f(0.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f,  0.0f, 0.0f), Vector2f(1.0f, 0.5f), Color4f(1.0f, 0.0f, 0.0f, 0.0f) }
    };

    static const u16 cIdx[2] = {
        0, 1
    };

    if (vtx != nullptr)
        MemUtil::copy(vtx, cVtx, sizeof(cVtx));

    if (idx != nullptr)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void setCubeVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[8] = {
        { Vector3f(-0.5f, -0.5f, -0.5f), Vector2f(0.0f, 0.0f), Color4f(1.0f / 3.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f(-0.5f,  0.5f, -0.5f), Vector2f(0.0f, 1.0f), Color4f(       0.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f(-0.5f,  0.5f,  0.5f), Vector2f(1.0f, 1.0f), Color4f(1.0f / 3.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f(-0.5f, -0.5f,  0.5f), Vector2f(1.0f, 0.0f), Color4f(2.0f / 3.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f, -0.5f,  0.5f), Vector2f(0.0f, 0.0f), Color4f(       1.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f,  0.5f,  0.5f), Vector2f(0.0f, 1.0f), Color4f(2.0f / 3.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f,  0.5f, -0.5f), Vector2f(1.0f, 1.0f), Color4f(1.0f / 3.0f, 0.0f, 0.0f, 0.0f) },
        { Vector3f( 0.5f, -0.5f, -0.5f), Vector2f(1.0f, 0.0f), Color4f(2.0f / 3.0f, 0.0f, 0.0f, 0.0f) }
    };

    static const u16 cIdx[36] = {
        2, 1, 0, 3,
        2, 0, 5, 2,
        3, 4, 5, 3,
        6, 5, 7, 7,
        5, 4, 1, 6,
        0, 6, 7, 0,
        0, 7, 3, 3,
        7, 4, 1, 2,
        6, 2, 5, 6
    };

    if (vtx != nullptr)
        MemUtil::copy(vtx, cVtx, sizeof(cVtx));

    if (idx != nullptr)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void setWireCubeVertex(Vertex* vtx, u16* idx)
{
    setCubeVertex(vtx, nullptr);

    static const u16 cIdx[17] = {
        0, 1, 2,
        3, 0, 7,
        6, 1, 2,
        5, 6, 7,
        4, 5, 4,
        3, 0

    };

    if (idx != nullptr)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void setSphereVertex(Vertex* vtx, u16* idx, s32 sizeX, s32 sizeY)
{
    if (vtx != nullptr)
    {
        for (s32 y = 0; y < sizeY; y++)
        {
            f32 angle_y = ((y + 1) / (sizeY + 1.0f) - 0.5f) * Mathf::deg2rad(180);

            f32 pos_y = Mathf::sin(angle_y) * 0.5f;
            f32 radius = Mathf::cos(angle_y) * 0.5f;

            for (s32 x = 0; x < sizeX; x++)
            {
                s32 pos = y * sizeX + x;
                f32 angle_x = Mathf::deg2rad(360) * x / sizeX;
                if (y % 2 == 0)
                    angle_x -= Mathf::deg2rad(360) / sizeX / 2;

                f32 pos_x = Mathf::cos(angle_x) * radius;
                f32 pos_z = Mathf::sin(angle_x) * radius;

                vtx[pos].pos.set(pos_x, pos_y, pos_z);
                vtx[pos].uv.set(pos_y + 0.5f, static_cast<f32>(x) / sizeX);
                vtx[pos].color.r = -pos_y + 0.5f;
            }
        }

        {
            s32 pos = sizeX * sizeY;

            vtx[pos].pos.set(0.0f, -0.5f, 0.0f);
            vtx[pos].uv.set(0.0f, 0.5f);
            vtx[pos].color.r = 1.0f;
        }

        {
            s32 pos = sizeX * sizeY + 1;

            vtx[pos].pos.set(0.0f, 0.5f, 0.0f);
            vtx[pos].uv.set(1.0f, 0.5f);
            vtx[pos].color.r = 0.0f;
        }
    }

    if (idx != nullptr)
    {
        for (s32 i = 0; i < sizeX; i++)
        {
          idx[i * 3 + 0] = sizeX * sizeY;
          idx[i * 3 + 1] = i;
          idx[i * 3 + 2] = (i + 1) % sizeX;
        }

        for (s32 y = 0; y < sizeY - 1; y++)
        {
            for (s32 x = 0; x < sizeX; x++)
            {
                s32 offset = y % 2;
                s32 pos = (y * sizeX * 6) + x * 6 + sizeX * 3;

                idx[pos + 0] = y * sizeX + x;
                idx[pos + 1] = (y + 1) * sizeX + ((x + offset) % sizeX);
                idx[pos + 2] = y * sizeX + ((x + 1) % sizeX);
                idx[pos + 3] = (y + 1) * sizeX + ((x + offset) % sizeX);
                idx[pos + 4] = (y + 1) * sizeX + ((x + 1 + offset) % sizeX);
                idx[pos + 5] = y * sizeX + ((x + 1) % sizeX);
            }
        }

        for (s32 i = 0; i < sizeX; i++)
        {
            s32 posOffs = 3 * sizeX * (sizeY - 1) * 2 + sizeX * 3;

            idx[i * 3 + 0 + posOffs] = sizeX * sizeY + 1;
            idx[i * 3 + 1 + posOffs] = sizeX * (sizeY - 1) + ((i + 1) % sizeX);
            idx[i * 3 + 2 + posOffs] = sizeX * (sizeY - 1) + i;
        }
    }
}

void setDiskVertex(Vertex* vtx, u16* idx, s32 divNum)
{
    if (vtx != nullptr)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            f32 angle = Mathf::deg2rad(360) * i / divNum;

            vtx[i].pos.x = Mathf::cos(angle) * 0.5f;
            vtx[i].pos.y = Mathf::sin(angle) * 0.5f;
            vtx[i].pos.z = 0.0f;
            vtx[i].uv.x = vtx[i].pos.x;
            vtx[i].uv.y = 1.0f - vtx[i].pos.y;
            vtx[i].color.r = 1.0f;
        }

        {
            s32 i = divNum;
            f32 angle = Mathf::deg2rad(360) * i / divNum;

            vtx[i].pos.set(0.0f, 0.0f, 0.0f);
            vtx[i].uv.set(0.5f, 0.5f);
            vtx[i].color.r = 0.0f;
        }
    }

    if (idx != nullptr)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            idx[i * 3 + 0] = i;
            idx[i * 3 + 1] = (i + 1) % divNum;
            idx[i * 3 + 2] = divNum;
        }
    }
}

void setCylinderVertex(Vertex* vtx, u16* idx, s32 divNum)
{
    s32 vtxNum = calcDiskVertexNum(divNum);
    s32 idxNum = calcDiskIndexNum(divNum);

    if (vtx != nullptr)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            f32 angle = Mathf::deg2rad(360) * i / divNum;

            vtx[i].pos.x = Mathf::cos(angle) * 0.5f;
            vtx[i].pos.z = -Mathf::sin(angle) * 0.5f;
            vtx[i].pos.y = 0.5f;
            vtx[i].uv.x = vtx[i].pos.x;
            vtx[i].uv.y = 1.0f - vtx[i].pos.z;
            vtx[i].color.r = 0.0f;

            s32 pos = i + vtxNum;

            vtx[pos].pos.x = Mathf::cos(angle) * 0.5f;
            vtx[pos].pos.z = -Mathf::sin(angle) * 0.5f;
            vtx[pos].pos.y = -0.5f;
            vtx[pos].uv.x = vtx[i].pos.x;
            vtx[pos].uv.y = 1.0f - vtx[i].pos.z;
            vtx[pos].color.r = 1.0f;
        }

        {
            s32 pos = divNum;

            vtx[pos].pos.set(0.0f, 0.5f, 0.0f);
            vtx[pos].uv.set(0.5f, 0.5f);
            vtx[pos].color.r = 0.0f;
        }

        {
            s32 pos = divNum + vtxNum;

            vtx[pos].pos.set(0.0f, -0.5f, 0.0f);
            vtx[pos].uv.set(0.5f, 0.5f);
            vtx[pos].color.r = 1.0f;
        }
    }

    if (idx != nullptr)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            idx[i * 3 + 0] = i;
            idx[i * 3 + 1] = (i + 1) - ((i + 1) % divNum);
            idx[i * 3 + 2] = divNum;

            s32 posOffs = idxNum;
            idx[i * 3 + 0 + posOffs] = i + vtxNum;
            idx[i * 3 + 1 + posOffs] = divNum + vtxNum;
            idx[i * 3 + 2 + posOffs] = ((i + 1) - ((i + 1) % divNum)) + vtxNum;
        }

        for (s32 i = 0; i < divNum; i++)
        {
            s32 posOffs = divNum * 6;

            idx[i * 6 + 0 + posOffs] = i;
            idx[i * 6 + 1 + posOffs] = i + vtxNum;
            idx[i * 6 + 2 + posOffs] = (i + 1) - ((i + 1) % divNum);
            idx[i * 6 + 3 + posOffs] = (i + 1) - ((i + 1) % divNum);
            idx[i * 6 + 4 + posOffs] = i + vtxNum;
            idx[i * 6 + 5 + posOffs] = ((i + 1) - ((i + 1) % divNum)) + vtxNum;
        }
    }
}

} } // namespace sead::PrimitiveRendererUtil

#include <utility/aglPrimitiveShape.h>
#include <utility/aglVertexAttributeHolder.h>

namespace agl { namespace utl {

SEAD_SINGLETON_DISPOSER_IMPL(VertexAttributeHolder)

VertexAttributeHolder::VertexAttributeHolder()
{
}

VertexAttributeHolder::~VertexAttributeHolder()
{
}

void VertexAttributeHolder::initialize(sead::Heap* heap)
{
    const PrimitiveShape* primitive_shape = PrimitiveShape::instance();

    mVertexAttribute[cAttribute_Quad].create(1, heap);
    mVertexAttribute[cAttribute_Quad].setVertexStream(0, &primitive_shape->mVtxBufferQuad, 0);
    mVertexAttribute[cAttribute_Quad].setUp();

    mVertexAttribute[cAttribute_Cube2].create(1, heap);
    mVertexAttribute[cAttribute_Cube2].setVertexStream(0, &primitive_shape->mVtxBufferCube, 0);
    mVertexAttribute[cAttribute_Cube2].setVertexStream(1, &primitive_shape->mVtxBufferCube, 1);
    mVertexAttribute[cAttribute_Cube2].setUp();

    mVertexAttribute[cAttribute_Cone].create(1, heap);
    mVertexAttribute[cAttribute_Cone].setVertexStream(0, &primitive_shape->mVtxBufferCone, 0);
    mVertexAttribute[cAttribute_Cone].setVertexStream(1, &primitive_shape->mVtxBufferCone, 1);
    mVertexAttribute[cAttribute_Cone].setUp();

    mVertexAttribute[cAttribute_Cylinder].create(1, heap);
    mVertexAttribute[cAttribute_Cylinder].setVertexStream(0, &primitive_shape->mVtxBufferCylinder, 0);
    mVertexAttribute[cAttribute_Cylinder].setVertexStream(1, &primitive_shape->mVtxBufferCylinder, 1);
    mVertexAttribute[cAttribute_Cylinder].setUp();

    mVertexAttribute[cAttribute_Sphere2].create(1, heap);
    mVertexAttribute[cAttribute_Sphere2].setVertexStream(0, &primitive_shape->mVtxBufferSphere, 0);
    mVertexAttribute[cAttribute_Sphere2].setVertexStream(1, &primitive_shape->mVtxBufferSphere, 1);
    mVertexAttribute[cAttribute_Sphere2].setUp();

    mVertexAttribute[cAttribute_QuadTriangle].create(1, heap);
    mVertexAttribute[cAttribute_QuadTriangle].setVertexStream(0, &primitive_shape->mVtxBufferQuadTriangle, 0);
    mVertexAttribute[cAttribute_QuadTriangle].setVertexStream(1, &primitive_shape->mVtxBufferQuadTriangle, 2);
    mVertexAttribute[cAttribute_QuadTriangle].setUp();

    mVertexAttribute[cAttribute_Circle].create(1, heap);
    mVertexAttribute[cAttribute_Circle].setVertexStream(0, &primitive_shape->mVtxBufferCircle, 0);
    mVertexAttribute[cAttribute_Circle].setVertexStream(1, &primitive_shape->mVtxBufferCircle, 2);
    mVertexAttribute[cAttribute_Circle].setUp();

    mVertexAttribute[cAttribute_Cube].create(1, heap);
    mVertexAttribute[cAttribute_Cube].setVertexStream(0, &primitive_shape->mVtxBufferCube, 0);
    mVertexAttribute[cAttribute_Cube].setUp();

    mVertexAttribute[cAttribute_Sphere].create(1, heap);
    mVertexAttribute[cAttribute_Sphere].setVertexStream(0, &primitive_shape->mVtxBufferSphere, 0);
    mVertexAttribute[cAttribute_Sphere].setUp();
}

} }


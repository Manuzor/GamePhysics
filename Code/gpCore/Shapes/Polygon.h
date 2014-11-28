#pragma once
#include "gpCore/Shapes/ShapeBase.h"

using gpPolygonShapeVertices = ezHybridArray<gpVec3, 8>;

class gpPolygonShape : public gpShapeBase
{
    // Data
    //////////////////////////////////////////////////////////////////////////
    gpPolygonShapeVertices m_Vertices;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpPolygonShapeVertices& gpVerticesOf(gpPolygonShape& polygon);
public:
    gpPolygonShape() : gpShapeBase(gpShapeType::Polygon) {}
};

EZ_FORCE_INLINE gpPolygonShapeVertices& gpVerticesOf(gpPolygonShape& polygon)
{
    return polygon.m_Vertices;
}
EZ_FORCE_INLINE const gpPolygonShapeVertices& gpVerticesOf(const gpPolygonShape& polygon)
{
    return gpVerticesOf(const_cast<gpPolygonShape&>(polygon));
}

EZ_FORCE_INLINE void gpConvertToBox(gpPolygonShape& polygon, const gpVec3& halfExtends)
{
    gpVerticesOf(polygon).SetCount(4);
    gpVerticesOf(polygon)[0].Set(-halfExtends.x,  halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[1].Set(-halfExtends.x, -halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[2].Set( halfExtends.x, -halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[3].Set( halfExtends.x,  halfExtends.y, 0.0f);
}

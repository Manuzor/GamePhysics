
namespace gpInternal
{
    struct gpPolygonShapeLayout : public gpShape
    {
        gpShapeVertices vertices;

        gpPolygonShapeLayout() : gpShape(gpShapeType::Polygon) {}
    };
}

EZ_FORCE_INLINE gpShapeVertices& gpVerticesOf(gpShape& polygon)
{
    EZ_ASSERT(gpTypeOf(polygon) == gpShapeType::Polygon, "Invalid type.");
    return reinterpret_cast<gpInternal::gpPolygonShapeLayout&>(polygon).vertices;
}

EZ_FORCE_INLINE const gpShapeVertices& gpVerticesOf(const gpShape& polygon)
{
    EZ_ASSERT(gpTypeOf(polygon) == gpShapeType::Polygon, "Invalid type.");
    return reinterpret_cast<const gpInternal::gpPolygonShapeLayout&>(polygon).vertices;
}

EZ_FORCE_INLINE void gpConvertToBox(gpShape& polygon, const gpVec3& halfExtends)
{
    gpVerticesOf(polygon).SetCount(4);
    gpVerticesOf(polygon)[0].Set(-halfExtends.x,  halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[1].Set(-halfExtends.x, -halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[2].Set( halfExtends.x, -halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[3].Set( halfExtends.x,  halfExtends.y, 0.0f);
}

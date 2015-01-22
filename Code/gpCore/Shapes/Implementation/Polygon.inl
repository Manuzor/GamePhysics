
EZ_FORCE_INLINE gpPolygonShape::VertexContainer& gpVerticesOf(gpShape& polygon)
{
    EZ_ASSERT(gpTypeOf(polygon) == gpShapeType::Polygon, "Invalid type.");
    return reinterpret_cast<gpPolygonShape&>(polygon).m_vertices;
}

EZ_FORCE_INLINE const gpPolygonShape::VertexContainer& gpVerticesOf(const gpShape& polygon)
{
    EZ_ASSERT(gpTypeOf(polygon) == gpShapeType::Polygon, "Invalid type.");
    return reinterpret_cast<const gpPolygonShape&>(polygon).m_vertices;
}

EZ_FORCE_INLINE void gpConvertToBox(gpShape& polygon, const gpVec3& halfExtends)
{
    gpVerticesOf(polygon).SetCount(4);
    gpVerticesOf(polygon)[0].Set(-halfExtends.x,  halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[1].Set(-halfExtends.x, -halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[2].Set( halfExtends.x, -halfExtends.y, 0.0f);
    gpVerticesOf(polygon)[3].Set( halfExtends.x,  halfExtends.y, 0.0f);
}

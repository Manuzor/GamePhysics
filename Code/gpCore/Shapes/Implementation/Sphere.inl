
EZ_FORCE_INLINE gpScalar& gpRadiusOf(gpShape& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Sphere, "Invalid type.");
    return reinterpret_cast<gpSphereShape&>(sphere).m_radius;
}

EZ_FORCE_INLINE gpScalar gpRadiusOf(const gpShape& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Sphere, "Invalid type.");
    return reinterpret_cast<const gpSphereShape&>(sphere).m_radius;
}

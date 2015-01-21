
namespace gpInternal
{
    struct gpSphereShapeLayout : public gpShape
    {
        gpScalar radius;

        gpSphereShapeLayout() : gpShape(gpShapeType::Sphere) {}
    };
}

EZ_FORCE_INLINE gpShapeType gpTypeOf(const gpShape& shape) { return shape.m_Type; }

EZ_FORCE_INLINE gpScalar& gpRadiusOf(gpShape& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Sphere, "Invalid type.");
    return reinterpret_cast<gpInternal::gpSphereShapeLayout&>(sphere).radius;
}

EZ_FORCE_INLINE gpScalar gpRadiusOf(const gpShape& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Sphere, "Invalid type.");
    return reinterpret_cast<const gpInternal::gpSphereShapeLayout&>(sphere).radius;
}


namespace gpInternal
{
    struct gpCircleShapeLayout : public gpShape
    {
        gpScalar radius;

        gpCircleShapeLayout() : gpShape(gpShapeType::Circle) {}
    };
}

EZ_FORCE_INLINE gpShapeType gpTypeOf(const gpShape& shape) { return shape.m_Type; }

EZ_FORCE_INLINE gpScalar& gpRadiusOf(gpShape& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Circle, "Invalid type.");
    return reinterpret_cast<gpInternal::gpCircleShapeLayout&>(sphere).radius;
}

EZ_FORCE_INLINE gpScalar gpRadiusOf(const gpShape& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Circle, "Invalid type.");
    return reinterpret_cast<const gpInternal::gpCircleShapeLayout&>(sphere).radius;
}

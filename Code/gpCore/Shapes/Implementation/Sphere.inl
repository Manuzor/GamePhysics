
namespace gpInternal
{
    struct gpCircleShapeLayout : public gpShapeBase
    {
        gpScalar radius;

        gpCircleShapeLayout() : gpShapeBase(gpShapeType::Circle) {}
    };
}

EZ_FORCE_INLINE gpShapeType gpTypeOf(const gpShapeBase& shape) { return shape.m_Type; }

EZ_FORCE_INLINE gpScalar& gpRadiusOf(gpShapeBase& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Circle, "Invalid type.");
    return reinterpret_cast<gpInternal::gpCircleShapeLayout&>(sphere).radius;
}

EZ_FORCE_INLINE gpScalar gpRadiusOf(const gpShapeBase& sphere)
{
    EZ_ASSERT(gpTypeOf(sphere) == gpShapeType::Circle, "Invalid type.");
    return reinterpret_cast<const gpInternal::gpCircleShapeLayout&>(sphere).radius;
}

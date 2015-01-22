#pragma once

// Shape type
//////////////////////////////////////////////////////////////////////////
namespace gpInternal
{
    struct gpShapeType
    {
        enum Enum
        {
            DefaultInit = -1,

            Point,
            Sphere,
            Line,
            Polygon,

            ENUM_COUNT
        };
    };
}
using gpShapeType = ezEnum<gpInternal::gpShapeType, ezUInt8>;

// Shape
//////////////////////////////////////////////////////////////////////////

/// \brief Base class for all shapes.
///
/// Shape instances are supposed to be used as gpShape, even if you have something more specialized,
/// such as gpPolygonShape. All algorithms defined for shapes accept a gpShape, not gpPolygonShape or gpSphereShape.
class GP_CoreAPI gpShape : public ezRefCounted
{
private:
    // Data
    //////////////////////////////////////////////////////////////////////////
    const gpShapeType m_Type;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    EZ_FORCE_INLINE friend gpShapeType gpTypeOf(const gpShape& shape) { return shape.m_Type; }

protected: // Construction
    gpShape(gpShapeType Type) : m_Type(Type) {}

public: // Construction
    gpShape(const gpShape&)        = delete;
    gpShape(gpShape&&)             = delete;
    void operator=(const gpShape&) = delete;
};

// Allocation
//////////////////////////////////////////////////////////////////////////

/// \brief Prevents direct allocations of gpShape.
///
/// Use gpNew and one of the specialized types to create a proper shape.
/// \see gpPointShape
/// \see gpSphereShape
/// \see gpPolygonShape
/// \see gpBoxShape
template<>
struct gpTypeAllocator<gpShape> {};

struct gpRefCountReleaseResult
{
    enum Outcome
    {
        Nothing,
        Deleted
    };

    Outcome value;

    gpRefCountReleaseResult(Outcome value) : value(value) {}

    bool WasDeleted() { return value == Deleted; }
};

/// \brief Not implemented on purpose. Use the other function.
void gpHandleUnreferencedObject(gpShape& shape);

/// \brief Deleted the shape and sets \a pShape to nullptr.
GP_CoreAPI void gpHandleUnreferencedObject(gpShape*& pShape);

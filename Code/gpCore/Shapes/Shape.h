#pragma once
#include "gpCore/Shapes/ShapeType.h"

class GP_CoreAPI gpShape : public ezRefCounted
{
private:
    // Data
    //////////////////////////////////////////////////////////////////////////
    const gpShapeType m_Type;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpShapeType gpTypeOf(const gpShape& shape);

public:
    gpShape(gpShapeType Type) : m_Type(Type) {}

    gpShape(const gpShape&)    = delete;
    gpShape(gpShape&&)         = delete;
    void operator=(const gpShape&) = delete;

    /// \note This function returns the same pointer for every call because a point-shape has no state to change.
    static gpShape* Point();
    static gpShape* Sphere(gpScalar radius);
    static gpShape* Box(const gpVec3& halfExtents);
};

using gpShapeVertices = ezHybridArray<gpVec3, 8>;

#include "gpCore/Shapes/Implementation/Sphere.inl"
#include "gpCore/Shapes/Implementation/Polygon.inl"

#include "gpCore/Shapes/Implementation/Allocation.inl"

#pragma once
#include "gpCore/Shapes/ShapeType.h"

class GP_CoreAPI gpShapeBase : public ezRefCounted
{
private:
    // Data
    //////////////////////////////////////////////////////////////////////////
    const gpShapeType m_Type;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpShapeType gpTypeOf(const gpShapeBase& shape);

public:
    gpShapeBase(gpShapeType Type) : m_Type(Type) {}

    gpShapeBase(const gpShapeBase&)    = delete;
    gpShapeBase(gpShapeBase&&)         = delete;
    void operator=(const gpShapeBase&) = delete;

    /// \note This function returns the same pointer for every call because a point-shape has no state to change.
    static gpShapeBase* Point();
    static gpShapeBase* NewSphere(gpScalar radius);
    static gpShapeBase* NewBox(const gpVec3& halfExtents);
};

using gpShapeVertices = ezHybridArray<gpVec3, 8>;

#include "gpCore/Shapes/Implementation/Sphere.inl"
#include "gpCore/Shapes/Implementation/Polygon.inl"

#include "gpCore/Shapes/Implementation/Allocation.inl"

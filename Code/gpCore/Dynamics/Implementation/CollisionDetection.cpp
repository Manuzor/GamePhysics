#include "gpCore/PCH.h"
#include "gpCore/Dynamics/CollisionDetection.h"

#include "gpCore/World/Particle.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/World/RigidBody.h"

#include "gpCore/Shapes/Circle.h"
#include "gpCore/Shapes/Rectangle.h"
#include "gpCore/Shapes/Polygon.h"

/// \brief Takes care of calling the proper gpAreColliding function, according to the actual type of \a unknown.
template<typename Type>
bool ShapeCallerHelper(const gpTransform& lhsTransform, const Type& lhsShape,
                       const gpTransform& rhsTransform, const gpShapeBase& rhsShape)
{
    switch(gpTypeOf(lhsShape))
    {
    case gpShapeType::Circle:
        return gpAreColliding(lhsTransform, lhsShape, rhsTransform, static_cast<const gpCircleShape&>(rhsShape));
    case gpShapeType::Polygon:
        return gpAreColliding(lhsTransform, lhsShape, rhsTransform, static_cast<const gpPolygonShape&>(rhsShape));
    default: break;
    }

    EZ_REPORT_FAILURE("Unsupported shape type in collision detection.");
    return false;
}

bool gpAreColliding(const gpRigidBody& lhs, const gpRigidBody& rhs)
{
    EZ_ASSERT(gpShapePtrOf(lhs) && gpShapePtrOf(rhs), "A rigid body needs a shape!");
    return gpAreColliding(gpTransformOf(lhs), Deref(gpShapePtrOf(lhs)),
                          gpTransformOf(rhs), Deref(gpShapePtrOf(rhs)));
}

bool gpAreColliding(const gpTransform& lhsTransform, const gpShapeBase& lhsShape,
                    const gpTransform& rhsTransform, const gpShapeBase& rhsShape)
{
    switch(gpTypeOf(lhsShape))
    {
    case gpShapeType::Circle:
        return ShapeCallerHelper(lhsTransform, static_cast<const gpCircleShape&>(lhsShape), rhsTransform, rhsShape);
    case gpShapeType::Polygon:
        return ShapeCallerHelper(lhsTransform, static_cast<const gpPolygonShape&>(lhsShape), rhsTransform, rhsShape);
    default: break;
    }

    EZ_REPORT_FAILURE("Unsupported shape type in collision detection.");
    return false;
}

bool gpAreColliding(const gpTransform& lhsTransform, const gpCircleShape& lhsShape,
                    const gpTransform& rhsTransform, const gpShapeBase&   rhsShape)
{
    return ShapeCallerHelper(lhsTransform, lhsShape,
                        rhsTransform, rhsShape);
}

bool gpAreColliding(const gpTransform& lhsTransform, const gpPolygonShape& lhsShape,
                    const gpTransform& rhsTransform, const gpShapeBase&   rhsShape)
{
    return ShapeCallerHelper(lhsTransform, lhsShape,
                        rhsTransform, rhsShape);
}

// Symmetric functions
//////////////////////////////////////////////////////////////////////////

bool gpAreColliding(const gpTransform& lhsTransform, const gpPolygonShape& lhsShape,
                    const gpTransform& rhsTransform, const gpCircleShape& rhsShape)
{
    // Swap operands
    // lhs, rhs => rhs, lhs
    return gpAreColliding(rhsTransform, rhsShape, lhsTransform, lhsShape);
}

// Most specialized functions
//////////////////////////////////////////////////////////////////////////

bool gpAreColliding(const gpTransform& lhsTransform, const gpCircleShape& lhsShape,
                    const gpTransform& rhsTransform, const gpCircleShape& rhsShape)
{
    // Calculate: |d2 - d1|² <= r1² + r2²

    auto d  = gpPositionOf(rhsTransform) - gpPositionOf(lhsTransform);
    auto r1 = gpRadiusOf(lhsShape);
    auto r2 = gpRadiusOf(rhsShape);
    return gpSquaredLengthOf(d) <= gpSquare(r1) + gpSquare(r2);
}

bool gpAreColliding(const gpTransform& lhsTransform, const gpPolygonShape& lhsShape,
                    const gpTransform& rhsTransform, const gpPolygonShape& rhsShape)
{
    GP_NotImplemented;
}

bool gpAreColliding(const gpTransform& lhsTransform, const gpCircleShape& lhsShape,
                    const gpTransform& rhsTransform, const gpPolygonShape& rhsShape)
{
    GP_NotImplemented;
}


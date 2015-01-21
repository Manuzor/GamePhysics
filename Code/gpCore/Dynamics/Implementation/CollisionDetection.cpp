#include "gpCore/PCH.h"
#include "gpCore/Dynamics/CollisionDetection.h"

#include "gpCore/World/Entity.h"
#include "gpCore/Shapes/Shape.h"

namespace
{
    bool CircleCircle(const gpTransform& lhsTransform, const gpShapeBase& lhsShape,
                      const gpTransform& rhsTransform, const gpShapeBase& rhsShape)
    {
        // Calculate: |d2 - d1|² <= r1² + r2²

        auto d  = gpPositionOf(rhsTransform) - gpPositionOf(lhsTransform);
        auto r1 = gpRadiusOf(lhsShape);
        auto r2 = gpRadiusOf(rhsShape);
        return gpSquaredLengthOf(d) <= gpSquare(r1) + gpSquare(r2);
    }
}

//////////////////////////////////////////////////////////////////////////

bool gpAreColliding(const gpEntity& lhs, const gpEntity& rhs)
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
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Circle:  return CircleCircle(lhsTransform, lhsShape, rhsTransform, rhsShape);
        case gpShapeType::Polygon: GP_NotImplemented;
        default: break;
        }
    case gpShapeType::Polygon:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Circle:  GP_NotImplemented;
        case gpShapeType::Polygon: GP_NotImplemented;
        default: break;
        }
    default: break;
    }

    EZ_REPORT_FAILURE("Unsupported shape type in collision detection.");
    return false;
}

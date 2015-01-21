#include "gpCore/PCH.h"
#include "gpCore/Dynamics/CollisionDetection.h"

#include "gpCore/World/Entity.h"
#include "gpCore/Shapes/Shape.h"

// Collision detection algorithms
//////////////////////////////////////////////////////////////////////////
namespace
{
    bool Point_Point(const gpTransform& lhs, const gpTransform& rhs)
    {
        // Calculate: p1 == p2
        auto& p1 = gpPositionOf(lhs);
        auto& p2 = gpPositionOf(rhs);
        return gpAreEqual(p1, p2);
    }

    bool Point_Circle(const gpTransform& pointTransform,
                      const gpTransform& circleTransform, const gpShapeBase& circle)
    {
        // Calculate: |p2 - p1|² <= r²

        auto& p1 = gpPositionOf(circleTransform);
        auto& p2 = gpPositionOf(pointTransform);
        auto r  = gpRadiusOf(circle);

        auto d = p2 - p1;
        return gpSquaredLengthOf(d) <= gpSquare(r);
    }

    bool Point_Polygon(const gpTransform& circleTransform,
                       const gpTransform& polygonTransform,
                       const gpShapeBase& polygon)
    {
        GP_NotImplemented;
    }

    bool Circle_Point(const gpTransform& circleTransform, const gpShapeBase& circle,
                      const gpTransform& pointTransform)
    {
        return Point_Circle(pointTransform, circleTransform, circle);
    }

    bool Circle_Circle(const gpTransform& lhsTransform, const gpShapeBase& lhsShape,
                       const gpTransform& rhsTransform, const gpShapeBase& rhsShape)
    {
        // Calculate: |p2 - p1|² <= (r1 + r2)²

        auto& p1 = gpPositionOf(lhsTransform);
        auto& p2 = gpPositionOf(rhsTransform);
        auto d   = p2 - p1;
        auto r1  = gpRadiusOf(lhsShape);
        auto r2  = gpRadiusOf(rhsShape);
        return gpSquaredLengthOf(d) <= gpSquare(r1 + r2);
    }

    bool Circle_Polygon(const gpTransform& circleTransform,  const gpShapeBase& circle,
                        const gpTransform& polygonTransform, const gpShapeBase& polygon)
    {
        GP_NotImplemented;
    }

    bool Polygon_Point(const gpTransform& polygonTransform,
                       const gpShapeBase& polygon,
                       const gpTransform& pointTransform)
    {
        return Point_Polygon(pointTransform, polygonTransform, polygon);
    }

    bool Polygon_Circle(const gpTransform& polygonTransform,
                        const gpShapeBase& polygon,
                        const gpTransform& sphereTransform,
                        const gpShapeBase& sphere)
    {
        return Circle_Polygon(polygonTransform, polygon, sphereTransform, sphere);
    }

    bool Polygon_Polygon(const gpTransform& lhsTransform,
                         const gpShapeBase& lhs,
                         const gpTransform& rhsTransform,
                         const gpShapeBase& rhs)
    {
        GP_NotImplemented;
    }
}

//////////////////////////////////////////////////////////////////////////

bool gpAreColliding(const gpEntity& lhs, const gpEntity& rhs)
{
    if (gpWorldPtrOf(lhs) != gpWorldPtrOf(rhs))
    {
        /// Objects that are not in the same simulation will never collide!
        /// \note It is allowed for both object to be in no simulation at all.
        return false;
    }

    return gpAreColliding(gpTransformOf(lhs), Deref(gpShapePtrOf(lhs)),
                          gpTransformOf(rhs), Deref(gpShapePtrOf(rhs)));
}

bool gpAreColliding(const gpTransform& lhsTransform, const gpShapeBase& lhsShape,
                    const gpTransform& rhsTransform, const gpShapeBase& rhsShape)
{
    switch(gpTypeOf(lhsShape))
    {
    case gpShapeType::Point:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Point_Point(  lhsTransform, rhsTransform);
        case gpShapeType::Circle:  return Point_Circle( lhsTransform, rhsTransform, rhsShape);
        case gpShapeType::Polygon: return Point_Polygon(lhsTransform, rhsTransform, rhsShape);
        default: break;
        }
    case gpShapeType::Circle:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Circle_Point(  lhsTransform, lhsShape, rhsTransform);
        case gpShapeType::Circle:  return Circle_Circle( lhsTransform, lhsShape, rhsTransform, rhsShape);
        case gpShapeType::Polygon: return Circle_Polygon(lhsTransform, lhsShape, rhsTransform, rhsShape);
        default: break;
        }
    case gpShapeType::Polygon:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Polygon_Point(  lhsTransform, lhsShape, rhsTransform);
        case gpShapeType::Circle:  return Polygon_Circle( lhsTransform, lhsShape, rhsTransform, rhsShape);
        case gpShapeType::Polygon: return Polygon_Polygon(lhsTransform, lhsShape, rhsTransform, rhsShape);
        default: break;
        }
    default: break;
    }

    EZ_REPORT_FAILURE("Unsupported shape type in collision detection.");
    return false;
}

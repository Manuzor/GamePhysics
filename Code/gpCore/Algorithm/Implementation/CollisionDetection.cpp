#include "gpCore/PCH.h"
#include "gpCore/Algorithm/CollisionDetection.h"

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

    bool Point_Sphere(const gpTransform& pointTransform,
                      const gpTransform& sphereTransform, const gpShape& sphere)
    {
        // Calculate: |p2 - p1|² < r²

        auto& p1 = gpPositionOf(sphereTransform);
        auto& p2 = gpPositionOf(pointTransform);
        auto r  = gpRadiusOf(sphere);

        auto d = p2 - p1;
        return gpSquaredLengthOf(d) < gpSquare(r);
    }

    bool Point_Polygon(const gpTransform& sphereTransform,
                       const gpTransform& polygonTransform,
                       const gpShape& polygon)
    {
        GP_NotImplemented;
    }

    bool Sphere_Point(const gpTransform& sphereTransform, const gpShape& sphere,
                      const gpTransform& pointTransform)
    {
        return Point_Sphere(pointTransform, sphereTransform, sphere);
    }

    bool Sphere_Sphere(const gpTransform& lhsTransform, const gpShape& lhsShape,
                       const gpTransform& rhsTransform, const gpShape& rhsShape)
    {
        // Calculate: |p2 - p1|² <= (r1 + r2)²

        auto& p1 = gpPositionOf(lhsTransform);
        auto& p2 = gpPositionOf(rhsTransform);
        auto d   = p2 - p1;
        auto r1  = gpRadiusOf(lhsShape);
        auto r2  = gpRadiusOf(rhsShape);
        return gpSquaredLengthOf(d) < gpSquare(r1 + r2);
    }

    bool Sphere_Polygon(const gpTransform& sphereTransform,  const gpShape& sphere,
                        const gpTransform& polygonTransform, const gpShape& polygon)
    {
        GP_NotImplemented;
    }

    bool Polygon_Point(const gpTransform& polygonTransform,
                       const gpShape& polygon,
                       const gpTransform& pointTransform)
    {
        return Point_Polygon(pointTransform, polygonTransform, polygon);
    }

    bool Polygon_Sphere(const gpTransform& polygonTransform,
                        const gpShape& polygon,
                        const gpTransform& sphereTransform,
                        const gpShape& sphere)
    {
        return Sphere_Polygon(polygonTransform, polygon, sphereTransform, sphere);
    }

    bool Polygon_Polygon(const gpTransform& lhsTransform,
                         const gpShape& lhs,
                         const gpTransform& rhsTransform,
                         const gpShape& rhs)
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

bool gpAreColliding(const gpTransform& lhsTransform, const gpShape& lhsShape,
                    const gpTransform& rhsTransform, const gpShape& rhsShape)
{
    switch(gpTypeOf(lhsShape))
    {
    case gpShapeType::Point:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Point_Point(  lhsTransform, rhsTransform);
        case gpShapeType::Sphere:  return Point_Sphere( lhsTransform, rhsTransform, rhsShape);
        case gpShapeType::Polygon: return Point_Polygon(lhsTransform, rhsTransform, rhsShape);
        default: break;
        }
    case gpShapeType::Sphere:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Sphere_Point(  lhsTransform, lhsShape, rhsTransform);
        case gpShapeType::Sphere:  return Sphere_Sphere( lhsTransform, lhsShape, rhsTransform, rhsShape);
        case gpShapeType::Polygon: return Sphere_Polygon(lhsTransform, lhsShape, rhsTransform, rhsShape);
        default: break;
        }
    case gpShapeType::Polygon:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Polygon_Point(  lhsTransform, lhsShape, rhsTransform);
        case gpShapeType::Sphere:  return Polygon_Sphere( lhsTransform, lhsShape, rhsTransform, rhsShape);
        case gpShapeType::Polygon: return Polygon_Polygon(lhsTransform, lhsShape, rhsTransform, rhsShape);
        default: break;
        }
    default: break;
    }

    EZ_REPORT_FAILURE("Unsupported shape type in collision detection.");
    return false;
}

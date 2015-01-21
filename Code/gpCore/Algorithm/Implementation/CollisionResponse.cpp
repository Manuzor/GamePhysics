#include "gpCore/PCH.h"
#include "gpCore/Algorithm/CollisionResponse.h"

#include "gpCore/World/Entity.h"
#include "gpCore/Shapes/Shape.h"

// Collision detection algorithms
//////////////////////////////////////////////////////////////////////////
namespace
{
    void Point_Point(const gpPhysicalProperties& lhs, const gpPhysicalProperties& rhs)
    {
        GP_NotImplemented;
    }

    void Point_Sphere(const gpPhysicalProperties& pointProps,
                      const gpPhysicalProperties& sphereProps, const gpShape& sphere)
    {
        GP_NotImplemented;
    }

    void Point_Polygon(const gpPhysicalProperties& sphereProps,
                       const gpPhysicalProperties& polygonProps,
                       const gpShape& polygon)
    {
        GP_NotImplemented;
    }

    void Sphere_Point(const gpPhysicalProperties& sphereProps, const gpShape& sphere,
                      const gpPhysicalProperties& pointProps)
    {
        return Point_Sphere(pointProps, sphereProps, sphere);
    }

    void Sphere_Sphere(const gpPhysicalProperties& lhsProps, const gpShape& lhsShape,
                       const gpPhysicalProperties& rhsProps, const gpShape& rhsShape)
    {
        GP_NotImplemented;
    }

    void Sphere_Polygon(const gpPhysicalProperties& sphereProps,  const gpShape& sphere,
                        const gpPhysicalProperties& polygonProps, const gpShape& polygon)
    {
        GP_NotImplemented;
    }

    void Polygon_Point(const gpPhysicalProperties& polygonProps,
                       const gpShape& polygon,
                       const gpPhysicalProperties& pointProps)
    {
        return Point_Polygon(pointProps, polygonProps, polygon);
    }

    void Polygon_Sphere(const gpPhysicalProperties& polygonProps,
                        const gpShape& polygon,
                        const gpPhysicalProperties& sphereProps,
                        const gpShape& sphere)
    {
        return Sphere_Polygon(polygonProps, polygon, sphereProps, sphere);
    }

    void Polygon_Polygon(const gpPhysicalProperties& lhsProps,
                         const gpShape& lhs,
                         const gpPhysicalProperties& rhsProps,
                         const gpShape& rhs)
    {
        GP_NotImplemented;
    }
}

//////////////////////////////////////////////////////////////////////////

void gpResolveCollision(const gpEntity& lhs, const gpEntity& rhs)
{
    EZ_ASSERT(gpWorldPtrOf(lhs) == gpWorldPtrOf(rhs),
              "Collisions between objects that are not in the same simulation can never be resolved! "
              "Note: It is allowed for both object to be in no simulation at all.");

    gpResolveCollision(gpPhysicalPropertiesOf(lhs), gpShapeOf(lhs),
                       gpPhysicalPropertiesOf(rhs), gpShapeOf(rhs));
}

void gpResolveCollision(const gpPhysicalProperties& lhsProps, const gpShape& lhsShape,
                        const gpPhysicalProperties& rhsProps, const gpShape& rhsShape)
{
    switch(gpTypeOf(lhsShape))
    {
    case gpShapeType::Point:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Point_Point(  lhsProps, rhsProps);
        case gpShapeType::Sphere:  return Point_Sphere( lhsProps, rhsProps, rhsShape);
        case gpShapeType::Polygon: return Point_Polygon(lhsProps, rhsProps, rhsShape);
        default: break;
        }
    case gpShapeType::Sphere:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Sphere_Point(  lhsProps, lhsShape, rhsProps);
        case gpShapeType::Sphere:  return Sphere_Sphere( lhsProps, lhsShape, rhsProps, rhsShape);
        case gpShapeType::Polygon: return Sphere_Polygon(lhsProps, lhsShape, rhsProps, rhsShape);
        default: break;
        }
    case gpShapeType::Polygon:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Polygon_Point(  lhsProps, lhsShape, rhsProps);
        case gpShapeType::Sphere:  return Polygon_Sphere( lhsProps, lhsShape, rhsProps, rhsShape);
        case gpShapeType::Polygon: return Polygon_Polygon(lhsProps, lhsShape, rhsProps, rhsShape);
        default: break;
        }
    default: break;
    }

    EZ_REPORT_FAILURE("Unsupported shape type in collision detection.");
}

#include "gpCore/PCH.h"
#include "gpCore/Algorithm/CollisionResponse.h"

#include "gpCore/World/Entity.h"
#include "gpCore/Shapes.h"

// Collision response algorithms
//////////////////////////////////////////////////////////////////////////
namespace
{
    void Sphere_Sphere(gpPhysicalProperties& lhsProps,
                       gpPhysicalProperties& rhsProps)
    {
        // Algorithm from http://www.gamasutra.com/view/feature/131424/pool_hall_lessons_fast_accurate_.php

        auto d = gpPositionOf(lhsProps) - gpPositionOf(rhsProps);
        if (gpIsZero(d))
            return;

        auto n = gpValueOf(d).GetNormalized();

        auto& v1 = gpValueOf(gpLinearVelocityOf(lhsProps));
        auto& v2 = gpValueOf(gpLinearVelocityOf(rhsProps));

        const gpScalar m1 = gpValueOf(gpMassOf(lhsProps));
        const gpScalar m2 = gpValueOf(gpMassOf(rhsProps));

        EZ_ASSERT(!gpIsZero(m1), "Invalid mass!");
        EZ_ASSERT(!gpIsZero(m2), "Invalid mass!");

        auto a1 = v1.Dot(n);
        auto a2 = v2.Dot(n);

        auto p = (2 * (a1 - a2))
              // ---------------
               / (m1 + m2);

        v1 -= p * m2 * n;
        v2 += p * m1 * n;
    }

    void Sphere_Point(gpPhysicalProperties& sphereProps,
                      gpPhysicalProperties& pointProps)
    {
        return Sphere_Sphere(sphereProps, pointProps);
    }

    void Sphere_Polygon(gpPhysicalProperties& sphereProps,  const gpShape& sphere,
                        gpPhysicalProperties& polygonProps, const gpShape& polygon)
    {
        GP_NotImplemented;
    }

    void Point_Point(gpPhysicalProperties& lhs, gpPhysicalProperties& rhs)
    {
        Sphere_Sphere(lhs, rhs);
    }

    void Point_Sphere(gpPhysicalProperties& pointProps,
                      gpPhysicalProperties& sphereProps)
    {
        return Sphere_Point(sphereProps, pointProps);
    }

    void Point_Polygon(gpPhysicalProperties& sphereProps,
                       gpPhysicalProperties& polygonProps,
                       const gpShape& polygon)
    {
        GP_NotImplemented;
    }

    void Polygon_Point(gpPhysicalProperties& polygonProps,
                       const gpShape& polygon,
                       gpPhysicalProperties& pointProps)
    {
        return Point_Polygon(pointProps, polygonProps, polygon);
    }

    void Polygon_Sphere(gpPhysicalProperties& polygonProps,
                        const gpShape& polygon,
                        gpPhysicalProperties& sphereProps,
                        const gpShape& sphere)
    {
        return Sphere_Polygon(polygonProps, polygon, sphereProps, sphere);
    }

    void Polygon_Polygon(gpPhysicalProperties& lhsProps,
                         const gpShape& lhs,
                         gpPhysicalProperties& rhsProps,
                         const gpShape& rhs)
    {
        ezLog::Dev("Implement me!");
    }
}

//////////////////////////////////////////////////////////////////////////

void gpResolveCollision(gpEntity& lhs, gpEntity& rhs)
{
    EZ_ASSERT(gpWorldPtrOf(lhs) == gpWorldPtrOf(rhs),
              "Collisions between objects that are not in the same simulation can never be resolved! "
              "Note: It is allowed for both object to be in no simulation at all.");

    gpResolveCollision(gpPhysicalPropertiesOf(lhs), gpShapeOf(lhs),
                       gpPhysicalPropertiesOf(rhs), gpShapeOf(rhs));
}

void gpResolveCollision(gpPhysicalProperties& lhsProps, const gpShape& lhsShape,
                        gpPhysicalProperties& rhsProps, const gpShape& rhsShape)
{
    switch(gpTypeOf(lhsShape))
    {
    case gpShapeType::Point:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Point_Point(  lhsProps, rhsProps);
        case gpShapeType::Sphere:  return Point_Sphere( lhsProps, rhsProps);
        case gpShapeType::Polygon: return Point_Polygon(lhsProps, rhsProps, rhsShape);
        default: break;
        }
    case gpShapeType::Sphere:
        switch(gpTypeOf(rhsShape))
        {
        case gpShapeType::Point:   return Sphere_Point(  lhsProps, rhsProps);
        case gpShapeType::Sphere:  return Sphere_Sphere( lhsProps, rhsProps);
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

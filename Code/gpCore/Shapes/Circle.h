#pragma once
#include "gpCore/Shapes/ShapeBase.h"
#include "gpCore/World/PhysicalProperties.h"

struct gpCircleShape : public gpShapeBase
{
    EZ_DECLARE_POD_TYPE();

    gpCircleShape() : gpShapeBase(gpShapeType::Circle) {}

    // Data
    //////////////////////////////////////////////////////////////////////////
    gpScalar m_fRadius = 1.0f;
};

EZ_FORCE_INLINE       gpScalar& gpRadiusOf(      gpCircleShape& circle) { return circle.m_fRadius; }
EZ_FORCE_INLINE const gpScalar& gpRadiusOf(const gpCircleShape& circle) { return circle.m_fRadius; }

bool gpContains(const gpPhysicalProperties& props, const gpCircleShape& Circle, const gpVec3& Point);

#include "gpCore/Shapes/Implementation/Circle.inl"

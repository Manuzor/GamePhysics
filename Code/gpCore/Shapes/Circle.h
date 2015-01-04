#pragma once
#include "gpCore/Shapes/ShapeBase.h"
#include "gpCore/World/PhysicalProperties.h"

class gpCircleShape : public gpShapeBase
{
public:
    EZ_DECLARE_POD_TYPE();

    gpCircleShape() : gpShapeBase(gpShapeType::Circle) {}

private:
    // Data
    //////////////////////////////////////////////////////////////////////////
    gpScalar m_fRadius = 1.0f;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpScalar& gpRadiusOf(gpCircleShape& circle);
};

EZ_FORCE_INLINE       gpScalar& gpRadiusOf(      gpCircleShape& circle) { return circle.m_fRadius; }
EZ_FORCE_INLINE const gpScalar& gpRadiusOf(const gpCircleShape& circle) { return gpRadiusOf(const_cast<gpCircleShape&>(circle)); }

bool gpContains(const gpPhysicalProperties& props, const gpCircleShape& Circle, const gpDisplacement& Point);

#include "gpCore/Shapes/Implementation/Circle.inl"

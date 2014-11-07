#pragma once
#include "gpCore/World/PhysicalProperties.h"

class GP_CoreAPI gpCircleShape
{
public:
    EZ_DECLARE_POD_TYPE();

    EZ_FORCE_INLINE gpScalar GetRadius() const { return m_fRadius; }
    EZ_FORCE_INLINE void SetRadius(gpScalar fRadius) { m_fRadius = fRadius; }

private:
    gpScalar m_fRadius = 1.0f;
};

bool gpContains(const gpPhysicalProperties* pProps, const gpCircleShape& Circle, const gpVec3& Point);

#include "gpCore/Shapes/Implementation/Circle.inl"

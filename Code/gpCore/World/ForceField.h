#pragma once
#include "gpCore/World/EntityBase.h"
#include "gpCore/Shapes/Circle.h"

class GP_CoreAPI gpForceFieldEntity : public gpEntityBase
{
public:
    gpForceFieldEntity() : gpEntityBase(gpEntityType::ForceField) {}

    EZ_FORCE_INLINE gpScalar GetForce() const { return m_fForce; }
    EZ_FORCE_INLINE void SetForce(gpScalar fForce) { m_fForce = fForce; }

    EZ_FORCE_INLINE gpScalar GetRadius() const { return m_Area.GetRadius(); }
    EZ_FORCE_INLINE void SetRadius(gpScalar fRadius) { m_Area.SetRadius(fRadius); }

    bool Contains(const gpVec3& Point) const { return gpContains(GetProperties(), m_Area, Point); }

private:

    gpScalar m_fForce = 0.0f;
    gpCircleShape m_Area;
};

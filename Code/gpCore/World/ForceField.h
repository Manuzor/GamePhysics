#pragma once
#include "gpCore/World/EntityBase.h"
#include "gpCore/Shapes/Circle.h"

struct GP_CoreAPI gpForceFieldEntity : public gpEntityBase
{
    gpForceFieldEntity() : gpEntityBase(gpEntityType::ForceField) {}

    // Data
    //////////////////////////////////////////////////////////////////////////
    gpScalar m_fForce = 0.0f;
    gpCircleShape m_Area;
};

EZ_FORCE_INLINE       gpScalar& gpForceOf(      gpForceFieldEntity& forceField) { return forceField.m_fForce; }
EZ_FORCE_INLINE const gpScalar& gpForceOf(const gpForceFieldEntity& forceField) { return forceField.m_fForce; }

EZ_FORCE_INLINE       gpScalar& gpRadiusOf(      gpForceFieldEntity& forceField) { return gpRadiusOf(forceField.m_Area); }
EZ_FORCE_INLINE const gpScalar& gpRadiusOf(const gpForceFieldEntity& forceField) { return gpRadiusOf(forceField.m_Area); }

EZ_FORCE_INLINE bool gpContains(const gpForceFieldEntity& forceField, const gpVec3& vPoint)
{
    return gpContains(gpPhysicalPropertiesOf(forceField), forceField.m_Area, vPoint);
}

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpForceFieldEntity& ForceField);

#pragma once
#include "gpCore/World/Entity.h"
#include "gpCore/Shapes/Circle.h"

class GP_CoreAPI gpForceFieldEntity : public gpEntity
{
    // Data
    //////////////////////////////////////////////////////////////////////////
    gpScalar m_fForceFactor = 0.0f;
    gpCircleShape m_Area;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpScalar& gpForceFactorOf(gpForceFieldEntity& forceField);
    friend gpScalar& gpRadiusOf(gpForceFieldEntity& forceField);
    friend bool gpContains(const gpForceFieldEntity& forceField, const gpDisplacement& vPoint);
};

EZ_FORCE_INLINE       gpScalar& gpForceFactorOf(      gpForceFieldEntity& forceField) { return forceField.m_fForceFactor; }
EZ_FORCE_INLINE const gpScalar& gpForceFactorOf(const gpForceFieldEntity& forceField)
{
    return gpForceFactorOf(const_cast<gpForceFieldEntity&>(forceField));
}

EZ_FORCE_INLINE gpScalar& gpRadiusOf(gpForceFieldEntity& forceField)
{
    return gpRadiusOf(forceField.m_Area);
}
EZ_FORCE_INLINE const gpScalar& gpRadiusOf(const gpForceFieldEntity& forceField)
{
    return gpRadiusOf(const_cast<gpForceFieldEntity&>(forceField));
}

EZ_FORCE_INLINE bool gpContains(const gpForceFieldEntity& forceField, const gpDisplacement& point)
{
    return gpContains(gpPhysicalPropertiesOf(forceField), forceField.m_Area, point);
}

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpForceFieldEntity& ForceField);

#pragma once
#include "gpCore/World/Entity.h"
#include "gpCore/Shapes/Shape.h"

class GP_CoreAPI gpForceFieldEntity : public gpEntity
{
    // Data
    //////////////////////////////////////////////////////////////////////////
    gpScalar m_fForceFactor = 0.0f;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpScalar& gpForceFactorOf(gpForceFieldEntity& forceField);
    friend gpScalar& gpRadiusOf(gpForceFieldEntity& forceField);

public:
    gpForceFieldEntity()
    {
        gpShapePtrOf(self) = gpShapeBase::NewSphere(1);
    }
};

// Allocation
//////////////////////////////////////////////////////////////////////////
namespace gpInternal
{
    template<>
    struct GP_CoreAPI gpTypeAllocator<gpForceFieldEntity>
    {
        static gpForceFieldEntity* New();
    };

    /// \note Delete & friends are not implemented on purpose.
}

// Functions
//////////////////////////////////////////////////////////////////////////

EZ_FORCE_INLINE       gpScalar& gpForceFactorOf(      gpForceFieldEntity& forceField) { return forceField.m_fForceFactor; }
EZ_FORCE_INLINE const gpScalar& gpForceFactorOf(const gpForceFieldEntity& forceField)
{
    return gpForceFactorOf(const_cast<gpForceFieldEntity&>(forceField));
}

EZ_FORCE_INLINE gpScalar& gpRadiusOf(gpForceFieldEntity& forceField)
{
    return gpRadiusOf(gpShapeOf(forceField));
}
EZ_FORCE_INLINE const gpScalar& gpRadiusOf(const gpForceFieldEntity& forceField)
{
    return gpRadiusOf(const_cast<gpForceFieldEntity&>(forceField));
}

GP_CoreAPI bool gpAffects(const gpForceFieldEntity& forceField, const gpEntity& entity);

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpForceFieldEntity& ForceField);

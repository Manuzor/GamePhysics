#pragma once
#include "gpCore/World/EntityType.h"
#include "gpCore/World/PhysicalProperties.h"

class gpWorld;

class GP_CoreAPI gpEntityBase : public ezRefCounted
{
    // Data
    //////////////////////////////////////////////////////////////////////////

    const gpEntityType m_Type;
    gpWorld* m_pWorld = nullptr;

    gpPhysicalProperties m_PhysicalProperties;
    ezString m_sName;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpEntityType gpTypeOf(const gpEntityBase& entity);
    friend gpWorld*& gpWorldPtrOf(gpEntityBase& entity);
    friend ezString& gpNameOf(gpEntityBase& entity);
    friend gpPhysicalProperties& gpPhysicalPropertiesOf(gpEntityBase& entity);

public:
    gpEntityBase(gpEntityType TheType) : m_Type(TheType) {}
    gpEntityBase(const gpEntityBase&) = delete;
    void operator=(const gpEntityBase&) = delete;
};

struct gpEntityDrawInfo
{
    ezColor m_Color = { 1, 1, 1, 1 };
    gpScalar m_fScale = 4.0f;
    ezColor m_LinearVelocityColor = { 0, 1, 1, 0.333f };
    ezAngle m_LinearVelocityArrowWingAngle = ezAngle::Degree(30);
    gpScalar m_fLinearVelocityArrowWingLength = 10.0f;
};

// Utility functions
//////////////////////////////////////////////////////////////////////////
void gpGetStats(ezStringBuilder& out_Stats, const gpEntityBase& entity);

EZ_FORCE_INLINE gpEntityType gpTypeOf(const gpEntityBase& entity) { return entity.m_Type; }

EZ_FORCE_INLINE       gpWorld*& gpWorldPtrOf(      gpEntityBase& entity) { return entity.m_pWorld; }
EZ_FORCE_INLINE const gpWorld*  gpWorldPtrOf(const gpEntityBase& entity)
{
    return gpWorldPtrOf(const_cast<gpEntityBase&>(entity));
}

EZ_FORCE_INLINE       ezString& gpNameOf(      gpEntityBase& entity) { return entity.m_sName; }
EZ_FORCE_INLINE const ezString& gpNameOf(const gpEntityBase& entity)
{
    return gpNameOf(const_cast<gpEntityBase&>(entity));
}

EZ_FORCE_INLINE gpPhysicalProperties& gpPhysicalPropertiesOf(gpEntityBase& entity)
{
    return entity.m_PhysicalProperties;
}
EZ_FORCE_INLINE const gpPhysicalProperties& gpPhysicalPropertiesOf(const gpEntityBase& entity)
{
    return gpPhysicalPropertiesOf(const_cast<gpEntityBase&>(entity));
}

EZ_FORCE_INLINE       gpTransform& gpTransformOf(      gpEntityBase& entity)
{
    return gpTransformOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpEntityBase& entity)
{
    return gpTransformOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE       gpVec3& gpPositionOf(gpEntityBase& entity)
{
    return gpPositionOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpVec3& gpPositionOf(const gpEntityBase& entity)
{
    return gpPositionOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE       gpMat3& gpRotationOf(gpEntityBase& entity)
{
    return gpRotationOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpMat3& gpRotationOf(const gpEntityBase& entity)
{
    return gpRotationOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE       gpVec3& gpLinearVelocityOf(gpEntityBase& entity)
{
    return gpLinearVelocityOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpVec3& gpLinearVelocityOf(const gpEntityBase& entity)
{
    return gpLinearVelocityOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE       gpScalar& gpGravityFactorOf(gpEntityBase& entity)
{
    return gpGravityFactorOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpEntityBase& entity)
{
    return gpGravityFactorOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE       gpScalar& gpMassOf(gpEntityBase& entity)
{
    return gpMassOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpScalar& gpMassOf(const gpEntityBase& entity)
{
    return gpMassOf(gpPhysicalPropertiesOf(entity));
}

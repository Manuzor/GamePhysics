#pragma once
#include "gpCore/World/EntityType.h"
#include "gpCore/World/PhysicalProperties.h"
#include "gpCore/Algorithm/Integrate.h"

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

EZ_FORCE_INLINE gpTransform& gpTransformOf(      gpEntityBase& entity)
{
    return gpTransformOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpEntityBase& entity)
{
    return gpTransformOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpDisplacement& gpPositionOf(gpEntityBase& entity)
{
    return gpPositionOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpDisplacement& gpPositionOf(const gpEntityBase& entity)
{
    return gpPositionOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpOrientation& gpRotationOf(gpEntityBase& entity)
{
    return gpRotationOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpOrientation& gpRotationOf(const gpEntityBase& entity)
{
    return gpRotationOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpLinearVelocity& gpLinearVelocityOf(gpEntityBase& entity)
{
    return gpLinearVelocityOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpLinearVelocity& gpLinearVelocityOf(const gpEntityBase& entity)
{
    return gpLinearVelocityOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpAngularMomentum& gpAngularMomentumOf(gpEntityBase& entity)
{
    return gpAngularMomentumOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpAngularMomentum& gpAngularMomentumOf(const gpEntityBase& entity)
{
    return gpAngularMomentumOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpInverseInertia& gpInverseInertiaOf(gpEntityBase& entity)
{
    return gpInverseInertiaOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpInverseInertia& gpInverseInertiaOf(const gpEntityBase& entity)
{
    return gpInverseInertiaOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpScalar& gpGravityFactorOf(gpEntityBase& entity)
{
    return gpGravityFactorOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpEntityBase& entity)
{
    return gpGravityFactorOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpMassSyncer gpMassOf(gpEntityBase& entity)
{
    return gpMassOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpMass gpMassOf(const gpEntityBase& entity)
{
    return gpMassOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpMassSyncer gpInverseMassOf(gpEntityBase& entity)
{
    return gpInverseMassOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpMass gpInverseMassOf(const gpEntityBase& entity)
{
    return gpInverseMassOf(gpPhysicalPropertiesOf(entity));
}

// Algorithms
//////////////////////////////////////////////////////////////////////////

EZ_FORCE_INLINE void gpApplyLinearImpulseTo(gpEntityBase& entity, const gpLinearVelocity& v)
{
    gpLinearVelocityOf(entity) = gpLinearVelocityOf(entity) + v;
}

EZ_FORCE_INLINE void gpApplyLinearImpulseTo(gpEntityBase& entity,
                                            const gpLinearVelocity& v,
                                            const gpDisplacement& applicationPosition)
{
    gpLinearVelocityOf(entity) = gpLinearVelocityOf(entity) + v;
}

EZ_FORCE_INLINE void gpApplyForceTo(gpEntityBase& entity, const gpForce& Force, gpTime dt)
{
    gpMass m = gpMassOf(entity);
    auto a = Force / m;
    auto v = a * dt;
    gpApplyLinearImpulseTo(entity, v);
}

EZ_FORCE_INLINE void gpApplyForceTo(gpEntityBase& entity,
                                    const gpForce& force,
                                    gpTime dt,
                                    const gpDisplacement& applicationPosition)
{
    auto& rotation = gpValueOf(gpRotationOf(entity));

    // Application point in world coordinates as pseudo-vector.
    auto P = rotation * gpValueOf(applicationPosition);
    // Force pseudo-vector in world coordinates.
    auto F = rotation * gpValueOf(force);

    // Torque as pseudo-vector
    auto result = P.Cross(F);

    if (gpIsZero(result))
        return;

    gpAngularMomentumOf(entity) += gpTorque(result) * dt;
}

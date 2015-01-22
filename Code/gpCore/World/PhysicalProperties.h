#pragma once

class gpPhysicalProperties
{
private: // Data
    gpTransform       m_Transform       = gpTransform(gpIdentity);
    gpLinearVelocity  m_LinearVelocity  = gpLinearVelocity(gpZero);
    gpAngularMomentum m_AngularMomentum = gpAngularMomentum(gpZero);
    gpInverseInertia  m_InverseInertia  = gpInverseInertia(gpZero);
    gpScalar          m_fGravityFactor  = 1.0f;
    gpMass            m_Mass            = gpMass(1.0f);

public: // Accessors / Friends


    EZ_FORCE_INLINE friend       gpTransform& gpTransformOf(      gpPhysicalProperties& props) { return props.m_Transform; }
    EZ_FORCE_INLINE friend const gpTransform& gpTransformOf(const gpPhysicalProperties& props) { return props.m_Transform; }

    EZ_FORCE_INLINE friend       gpLinearVelocity& gpLinearVelocityOf(      gpPhysicalProperties& props) { return props.m_LinearVelocity; }
    EZ_FORCE_INLINE friend const gpLinearVelocity& gpLinearVelocityOf(const gpPhysicalProperties& props) { return props.m_LinearVelocity; }

    EZ_FORCE_INLINE friend       gpAngularMomentum& gpAngularMomentumOf(      gpPhysicalProperties& props) { return props.m_AngularMomentum; }
    EZ_FORCE_INLINE friend const gpAngularMomentum& gpAngularMomentumOf(const gpPhysicalProperties& props) { return props.m_AngularMomentum; }

    EZ_FORCE_INLINE friend       gpInverseInertia& gpInverseInertiaOf(      gpPhysicalProperties& props) { return props.m_InverseInertia; }
    EZ_FORCE_INLINE friend const gpInverseInertia& gpInverseInertiaOf(const gpPhysicalProperties& props) { return props.m_InverseInertia; }

    EZ_FORCE_INLINE friend       gpScalar& gpGravityFactorOf(      gpPhysicalProperties& props) { return props.m_fGravityFactor; }
    EZ_FORCE_INLINE friend const gpScalar& gpGravityFactorOf(const gpPhysicalProperties& props) { return props.m_fGravityFactor; }

    EZ_FORCE_INLINE friend       gpMass& gpMassOf(      gpPhysicalProperties& props) { return props.m_Mass; }
    EZ_FORCE_INLINE friend const gpMass& gpMassOf(const gpPhysicalProperties& props) { return props.m_Mass; }
};

EZ_FORCE_INLINE       gpDisplacement& gpPositionOf(      gpPhysicalProperties& props) { return gpPositionOf(gpTransformOf(props)); }
EZ_FORCE_INLINE const gpDisplacement& gpPositionOf(const gpPhysicalProperties& props) { return gpPositionOf(gpTransformOf(props)); }

EZ_FORCE_INLINE       gpOrientation& gpRotationOf(      gpPhysicalProperties& props) { return gpRotationOf(gpTransformOf(props)); }
EZ_FORCE_INLINE const gpOrientation& gpRotationOf(const gpPhysicalProperties& props) { return gpRotationOf(gpTransformOf(props)); }

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props);

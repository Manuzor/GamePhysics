#pragma once

class gpPhysicalProperties
{
public: // Data
    gpTransform       m_Transform       = gpTransform(gpIdentity);
    gpLinearVelocity  m_LinearVelocity  = gpLinearVelocity(gpZero);
    gpAngularMomentum m_AngularMomentum = gpAngularMomentum(gpZero);
    gpInverseInertia  m_InverseInertia  = gpInverseInertia(gpZero);
    gpScalar          m_fGravityFactor  = 1.0f;
    gpMass            m_Mass            = gpMass(1.0f);
    gpMass            m_InverseMass     = gpMass(1.0f);
};

EZ_FORCE_INLINE       gpTransform& gpTransformOf(      gpPhysicalProperties& props) { return props.m_Transform; }
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpPhysicalProperties& props) { return props.m_Transform; }

EZ_FORCE_INLINE       gpDisplacement& gpPositionOf(      gpPhysicalProperties& props) { return gpPositionOf(gpTransformOf(props)); }
EZ_FORCE_INLINE const gpDisplacement& gpPositionOf(const gpPhysicalProperties& props) { return gpPositionOf(gpTransformOf(props)); }

EZ_FORCE_INLINE       gpOrientation& gpRotationOf(      gpPhysicalProperties& props) { return gpRotationOf(gpTransformOf(props)); }
EZ_FORCE_INLINE const gpOrientation& gpRotationOf(const gpPhysicalProperties& props) { return gpRotationOf(gpTransformOf(props)); }

EZ_FORCE_INLINE       gpLinearVelocity& gpLinearVelocityOf(      gpPhysicalProperties& props) { return props.m_LinearVelocity; }
EZ_FORCE_INLINE const gpLinearVelocity& gpLinearVelocityOf(const gpPhysicalProperties& props) { return props.m_LinearVelocity; }

EZ_FORCE_INLINE       gpAngularMomentum& gpAngularMomentumOf(      gpPhysicalProperties& props) { return props.m_AngularMomentum; }
EZ_FORCE_INLINE const gpAngularMomentum& gpAngularMomentumOf(const gpPhysicalProperties& props) { return props.m_AngularMomentum; }

EZ_FORCE_INLINE       gpInverseInertia& gpInverseInertiaOf(      gpPhysicalProperties& props) { return props.m_InverseInertia; }
EZ_FORCE_INLINE const gpInverseInertia& gpInverseInertiaOf(const gpPhysicalProperties& props) { return props.m_InverseInertia; }

EZ_FORCE_INLINE       gpScalar& gpGravityFactorOf(      gpPhysicalProperties& props) { return props.m_fGravityFactor; }
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpPhysicalProperties& props) { return props.m_fGravityFactor; }

EZ_FORCE_INLINE       gpMassSyncer gpMassOf(      gpPhysicalProperties& props) { return { props.m_Mass, props.m_InverseMass }; }
EZ_FORCE_INLINE const gpMass&      gpMassOf(const gpPhysicalProperties& props) { return props.m_Mass; }

EZ_FORCE_INLINE       gpMassSyncer gpInverseMassOf(      gpPhysicalProperties& props) { return { props.m_InverseMass, props.m_Mass }; }
EZ_FORCE_INLINE const gpMass&      gpInverseMassOf(const gpPhysicalProperties& props) { return props.m_InverseMass; }

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props);

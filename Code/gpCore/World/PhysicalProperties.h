#pragma once

struct gpPhysicalProperties
{
#if __INTELLISENSE__
    gpTransform m_Transform;
#else
    gpTransform m_Transform = { gpVec3::ZeroVector(), gpQuaternion::IdentityQuaternion() };
#endif
    gpVelocityUnit m_LinearVelocity  = gpVelocity(gpVec3{ 0, 0, 0 });
    gpVelocityUnit m_AngularVelocity = gpVelocity(gpVec3{ 0, 0, 0 });
    gpScalar       m_fGravityFactor  = 1.0f;
    gpMassUnit     m_Mass            = gpMass(1.0f);
    gpMassUnit     m_InverseMass     = gpMass(1.0f);
};

EZ_FORCE_INLINE       gpTransform& gpTransformOf(      gpPhysicalProperties& props) { return props.m_Transform; }
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpPhysicalProperties& props) { return props.m_Transform; }

EZ_FORCE_INLINE       gpVec3& gpPositionOf(      gpPhysicalProperties& props) { return gpTransformOf(props).m_vPosition; }
EZ_FORCE_INLINE const gpVec3& gpPositionOf(const gpPhysicalProperties& props) { return gpTransformOf(props).m_vPosition; }

EZ_FORCE_INLINE       gpMat3& gpRotationOf(      gpPhysicalProperties& props) { return gpTransformOf(props).m_Rotation; }
EZ_FORCE_INLINE const gpMat3& gpRotationOf(const gpPhysicalProperties& props) { return gpTransformOf(props).m_Rotation; }

EZ_FORCE_INLINE       gpVelocityUnit& gpLinearVelocityOf(      gpPhysicalProperties& props) { return props.m_LinearVelocity; }
EZ_FORCE_INLINE const gpVelocityUnit& gpLinearVelocityOf(const gpPhysicalProperties& props) { return props.m_LinearVelocity; }

EZ_FORCE_INLINE       gpVelocityUnit& gpAngularVelocityOf(      gpPhysicalProperties& props) { return props.m_AngularVelocity; }
EZ_FORCE_INLINE const gpVelocityUnit& gpAngularVelocityOf(const gpPhysicalProperties& props) { return props.m_AngularVelocity; }

EZ_FORCE_INLINE       gpScalar& gpGravityFactorOf(      gpPhysicalProperties& props) { return props.m_fGravityFactor; }
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpPhysicalProperties& props) { return props.m_fGravityFactor; }

EZ_FORCE_INLINE       gpMassSyncer gpMassOf(      gpPhysicalProperties& props) { return { props.m_Mass, props.m_InverseMass }; }
EZ_FORCE_INLINE const gpMassUnit   gpMassOf(const gpPhysicalProperties& props) { return props.m_Mass; }

EZ_FORCE_INLINE       gpMassSyncer gpInverseMassOf(      gpPhysicalProperties& props) { return { props.m_InverseMass, props.m_Mass }; }
EZ_FORCE_INLINE const gpMassUnit   gpInverseMassOf(const gpPhysicalProperties& props) { return props.m_InverseMass; }

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props);

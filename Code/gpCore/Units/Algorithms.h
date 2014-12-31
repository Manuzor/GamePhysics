#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"
#include "gpCore/Units/Velocity.h"
#include "gpCore/Units/Acceleration.h"
#include "gpCore/Units/Force.h"

// Scaling
//////////////////////////////////////////////////////////////////////////

inline gpMassUnit operator *(const gpMassUnit& m, gpScalar factor) { return gpMass(gpValueOf(m) * factor); }
inline gpMassUnit operator *(gpScalar factor, const gpMassUnit& m) { return m * factor; }
inline gpMassUnit operator /(const gpMassUnit& m, gpScalar factor) { return gpMass(gpValueOf(m) * ezMath::Invert(factor)); }

inline gpDisplacementUnit operator *(const gpDisplacementUnit& d, gpScalar factor) { return gpDisplacement(gpValueOf(d) * factor); }
inline gpDisplacementUnit operator *(gpScalar factor, const gpDisplacementUnit& d) { return d * factor; }
inline gpDisplacementUnit operator /(const gpDisplacementUnit& d, gpScalar factor) { return d * ezMath::Invert(factor); }

inline gpAccelerationUnit operator *(const gpAccelerationUnit& a, gpScalar factor) { return gpAcceleration(gpValueOf(a) * factor); }
inline gpAccelerationUnit operator *(gpScalar factor, const gpAccelerationUnit& a) { return a * factor; }
inline gpAccelerationUnit operator /(const gpAccelerationUnit& a, gpScalar factor) { return gpAcceleration(gpValueOf(a) * ezMath::Invert(factor)); }

inline gpForceUnit operator *(const gpForceUnit& f, gpScalar factor) { return gpForce(gpValueOf(f) * factor); }
inline gpForceUnit operator *(gpScalar factor, const gpForceUnit& f) { return f * factor; }
inline gpForceUnit operator /(const gpForceUnit& f, gpScalar factor) { return gpForce(gpValueOf(f) * ezMath::Invert(factor)); }

//////////////////////////////////////////////////////////////////////////

/// \brief Construct a DisplacementUnit from a VelocityUnit and a given delta-time
/// \example d = v * (dt / t)
inline gpDisplacementUnit operator *(const gpVelocityUnit& v, gpTimeUnit dt)
{
    return gpDisplacement(gpValueOf(v) * (gpScalar)gpValueOf(dt));
}
inline gpDisplacementUnit operator *(gpTimeUnit dt, const gpVelocityUnit& v) { return v * dt; }

inline gpVelocityUnit operator *(const gpAccelerationUnit& a, gpTimeUnit dt)
{
    return gpVelocity(gpValueOf(a) * (gpScalar)gpValueOf(dt));
}
inline gpVelocityUnit operator *(gpTimeUnit dt, const gpAccelerationUnit& a) { return a * dt; }

inline gpForceUnit operator *(gpMassUnit m, const gpAccelerationUnit& a) { return gpForce(m, a); }
inline gpForceUnit operator *(const gpAccelerationUnit& a, gpMassUnit m) { return m * a; }
inline gpAccelerationUnit operator /(const gpForceUnit& f, gpMassUnit m)
{
    return gpAcceleration(gpValueOf(f) * ezMath::Invert(gpValueOf(m)));
}

//////////////////////////////////////////////////////////////////////////
// Equality
//////////////////////////////////////////////////////////////////////////

inline
bool gpIsEqual(const gpMassUnit& lhs, const gpMassUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return ezMath::IsEqual(gpValueOf(lhs), gpValueOf(rhs), fEpsilon);
}

inline
bool gpIsEqual(const gpTimeUnit& lhs, const gpTimeUnit& rhs, double fEpsilon = ezMath::BasicType<double>::DefaultEpsilon())
{
    return ezMath::IsEqual(gpValueOf(lhs), gpValueOf(rhs), fEpsilon);
}

inline
bool gpIsEqual(const gpDisplacementUnit& lhs, const gpDisplacementUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return gpValueOf(lhs).IsEqual(gpValueOf(rhs), fEpsilon);
}

inline
bool gpIsEqual(const gpVelocityUnit& lhs, const gpVelocityUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return gpValueOf(lhs).IsEqual(gpValueOf(rhs), fEpsilon);
}

inline
bool gpIsEqual(const gpAccelerationUnit& lhs, const gpAccelerationUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return gpValueOf(lhs).IsEqual(gpValueOf(rhs), fEpsilon);
}

inline
bool gpIsEqual(const gpForceUnit& lhs, const gpForceUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return gpValueOf(lhs).IsEqual(gpValueOf(rhs), fEpsilon);
}

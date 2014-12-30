#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"
#include "gpCore/Units/Velocity.h"
#include "gpCore/Units/Acceleration.h"
#include "gpCore/Units/Force.h"

// Scaling
//////////////////////////////////////////////////////////////////////////

inline MassUnit operator *(const MassUnit& m, gpScalar factor) { return Mass(ValueOf(m) * factor); }
inline MassUnit operator *(gpScalar factor, const MassUnit& m) { return m * factor; }
inline MassUnit operator /(const MassUnit& m, gpScalar factor) { return Mass(ValueOf(m) * ezMath::Invert(factor)); }

inline DisplacementUnit operator *(const DisplacementUnit& d, gpScalar factor) { return Displacement(ValueOf(d) * factor); }
inline DisplacementUnit operator *(gpScalar factor, const DisplacementUnit& d) { return d * factor; }
inline DisplacementUnit operator /(const DisplacementUnit& d, gpScalar factor) { return d * ezMath::Invert(factor); }

inline AccelerationUnit operator *(const AccelerationUnit& a, gpScalar factor) { return Acceleration(ValueOf(a) * factor); }
inline AccelerationUnit operator *(gpScalar factor, const AccelerationUnit& a) { return a * factor; }
inline AccelerationUnit operator /(const AccelerationUnit& a, gpScalar factor) { return Acceleration(ValueOf(a) * ezMath::Invert(factor)); }

inline ForceUnit operator *(const ForceUnit& f, gpScalar factor) { return Force(ValueOf(f) * factor); }
inline ForceUnit operator *(gpScalar factor, const ForceUnit& f) { return f * factor; }
inline ForceUnit operator /(const ForceUnit& f, gpScalar factor) { return Force(ValueOf(f) * ezMath::Invert(factor)); }

//////////////////////////////////////////////////////////////////////////

/// \brief Construct a DisplacementUnit from a VelocityUnit and a given delta-time
/// \example d = v * (dt / t)
inline DisplacementUnit operator *(const VelocityUnit& v, TimeUnit dt)
{
    return Displacement(ValueOf(v) * (gpScalar)ValueOf(dt));
}
inline DisplacementUnit operator *(TimeUnit dt, const VelocityUnit& v) { return v * dt; }

inline VelocityUnit operator *(const AccelerationUnit& a, TimeUnit dt)
{
    return Velocity(ValueOf(a) * (gpScalar)ValueOf(dt));
}
inline VelocityUnit operator *(TimeUnit dt, const AccelerationUnit& a) { return a * dt; }

inline ForceUnit operator *(MassUnit m, const AccelerationUnit& a) { return Force(m, a); }
inline ForceUnit operator *(const AccelerationUnit& a, MassUnit m) { return m * a; }
inline AccelerationUnit operator /(const ForceUnit& f, MassUnit m)
{
    return Acceleration(ValueOf(f) * ezMath::Invert(ValueOf(m)));
}

//////////////////////////////////////////////////////////////////////////
// Equality
//////////////////////////////////////////////////////////////////////////

inline
bool IsEqual(const MassUnit& lhs, const MassUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return ezMath::IsEqual(ValueOf(lhs), ValueOf(rhs), fEpsilon);
}

inline
bool IsEqual(const TimeUnit& lhs, const TimeUnit& rhs, double fEpsilon = ezMath::BasicType<double>::DefaultEpsilon())
{
    return ezMath::IsEqual(ValueOf(lhs), ValueOf(rhs), fEpsilon);
}

inline
bool IsEqual(const DisplacementUnit& lhs, const DisplacementUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return ValueOf(lhs).IsEqual(ValueOf(rhs), fEpsilon);
}

inline
bool IsEqual(const VelocityUnit& lhs, const VelocityUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return ValueOf(lhs).IsEqual(ValueOf(rhs), fEpsilon);
}

inline
bool IsEqual(const AccelerationUnit& lhs, const AccelerationUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return ValueOf(lhs).IsEqual(ValueOf(rhs), fEpsilon);
}

inline
bool IsEqual(const ForceUnit& lhs, const ForceUnit& rhs, gpScalar fEpsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon())
{
    return ValueOf(lhs).IsEqual(ValueOf(rhs), fEpsilon);
}

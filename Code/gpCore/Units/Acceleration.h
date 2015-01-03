#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Acceleration => Distance per Squared-Time
/// \example m/(s*s) (meter per second per second)
class gpAcceleration
{
    gpVec3 m_Value;

    gpAcceleration(const gpVec3& Value) : m_Value{ Value } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    gpAcceleration gpAcceleration(const gpVec3& Value) { return gpAcceleration{ Value }; }

    /// Constructor
    EZ_FORCE_INLINE friend
    gpAcceleration gpAcceleration(const gpDisplacement& d, gpTime t)
    {
        return gpAcceleration{ gpValueOf(d) * (gpScalar)ezMath::Invert(gpValueOf(t) * gpValueOf(t)) };
    }

    EZ_FORCE_INLINE friend       gpVec3& gpValueOf(      gpAcceleration& a) { return a.m_Value; }
    EZ_FORCE_INLINE friend const gpVec3& gpValueOf(const gpAcceleration& a) { return a.m_Value; }

public:
    gpAcceleration() {}
};

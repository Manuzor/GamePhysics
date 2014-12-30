#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Acceleration => Distance per Squared-Time
/// \example m/(s*s) (meter per second per second)
class AccelerationUnit
{
    gpVec3 m_Value;

    AccelerationUnit(const gpVec3& Value) : m_Value{ Value } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    AccelerationUnit Acceleration(const gpVec3& Value) { return AccelerationUnit{ Value }; }

    /// Constructor
    EZ_FORCE_INLINE friend
    AccelerationUnit Acceleration(const DisplacementUnit& d, TimeUnit t)
    {
        return AccelerationUnit{ ValueOf(d) * (gpScalar)ezMath::Invert(ValueOf(t) * ValueOf(t)) };
    }

    EZ_FORCE_INLINE friend
    gpVec3 ValueOf(const AccelerationUnit& a) { return a.m_Value; }
};

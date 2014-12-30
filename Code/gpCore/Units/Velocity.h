#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Represents d / t, d: displacement, t: time => Distance per Time unit
/// \example m/s (meter per second)
class VelocityUnit
{
    gpVec3 m_Value;

    VelocityUnit(const gpVec3& Value) : m_Value{ Value } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    VelocityUnit Velocity(const gpVec3& Value) { return VelocityUnit{ Value }; }

    /// Constructor
    EZ_FORCE_INLINE friend
    VelocityUnit Velocity(const DisplacementUnit& d, TimeUnit t)
    {
        return Velocity(ValueOf(d) * (gpScalar)ezMath::Invert(ValueOf(t)));
    }

    EZ_FORCE_INLINE friend
    gpVec3 ValueOf(const VelocityUnit& v) { return v.m_Value; }
};

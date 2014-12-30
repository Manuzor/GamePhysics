#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Represents d / t, d: displacement, t: time => Distance per Time unit
/// \example m/s (meter per second)
class VelocityUnit
{
    DisplacementUnit m_Displacement;
    TimeUnit m_Time;

    VelocityUnit(const DisplacementUnit& d, TimeUnit t) : m_Displacement(d), m_Time(t) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    VelocityUnit Velocity(const DisplacementUnit& d, TimeUnit t) { return VelocityUnit{ d, t }; }

    EZ_FORCE_INLINE friend
    DisplacementUnit DisplacementOf(const VelocityUnit& v) { return v.m_Displacement; }

    EZ_FORCE_INLINE friend
    TimeUnit TimeOf(const VelocityUnit& v) { return v.m_Time; }
};

#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Acceleration => Distance per Squared-Time
/// \example m/(s*s) (meter per second per second)
class AccelerationUnit
{
    DisplacementUnit m_Displacement;
    TimeUnit m_Time;

    AccelerationUnit(const DisplacementUnit& d, TimeUnit t) : m_Displacement(d), m_Time(t) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor a = d / (t * t)
    EZ_FORCE_INLINE friend
    AccelerationUnit Acceleration(const DisplacementUnit& d, TimeUnit t) { return { d, t }; }

    EZ_FORCE_INLINE friend
    DisplacementUnit DisplacementOf(const AccelerationUnit& a) { return a.m_Displacement; }

    /// \remarks Does not return the squared time!
    EZ_FORCE_INLINE friend
    TimeUnit TimeOf(const AccelerationUnit& a) { return a.m_Time; }
};

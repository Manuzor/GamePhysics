#pragma once

/// \brief Construct a DisplacementUnit from a VelocityUnit and a given delta-time
/// \example d = v * (dt / t)
inline gpDisplacement operator *(const gpVelocity& v, gpTime dt)
{
    return gpDisplacement(gpValueOf(v) * (gpScalar)gpValueOf(dt));
}
inline gpDisplacement operator *(gpTime dt, const gpVelocity& v) { return v * dt; }

inline gpVelocity operator *(const gpAcceleration& a, gpTime dt)
{
    return gpVelocity(gpValueOf(a) * (gpScalar)gpValueOf(dt));
}
inline gpVelocity operator *(gpTime dt, const gpAcceleration& a) { return a * dt; }
inline gpVelocity operator /(const gpDisplacement& d, gpTime dt)
{
    return gpVelocity(gpValueOf(d) / (gpScalar)gpValueOf(dt));
}

inline gpForce operator *(gpMass m, const gpAcceleration& a) { return gpForce(gpValueOf(m) * gpValueOf(a)); }
inline gpForce operator *(const gpAcceleration& a, gpMass m) { return m * a; }
inline gpAcceleration operator /(const gpForce& f, gpMass m)
{
    return gpAcceleration(gpValueOf(f) / gpValueOf(m));
}
inline gpAcceleration operator /(const gpVelocity& v, gpTime t)
{
    return gpAcceleration(gpValueOf(v) / (gpScalar)gpValueOf(t));
}

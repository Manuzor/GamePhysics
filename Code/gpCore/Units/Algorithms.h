#pragma once

/// d = v * dt
inline gpDisplacement operator *(const gpLinearVelocity& v, gpTime dt)
{
    return gpDisplacement(gpValueOf(v) * (gpScalar)gpValueOf(dt));
}

/// d = dt * v
inline gpDisplacement operator *(gpTime dt, const gpLinearVelocity& v) { return v * dt; }

/// v = a * dt
inline gpLinearVelocity operator *(const gpLinearAcceleration& a, gpTime dt)
{
    return gpLinearVelocity(gpValueOf(a) * (gpScalar)gpValueOf(dt));
}

/// v = dt * a
inline gpLinearVelocity operator *(gpTime dt, const gpLinearAcceleration& a) { return a * dt; }

/// v = d / dt
inline gpLinearVelocity operator /(const gpDisplacement& d, gpTime dt)
{
    return gpLinearVelocity(gpValueOf(d) / (gpScalar)gpValueOf(dt));
}

/// F = m * a
inline gpForce operator *(gpMass m, const gpLinearAcceleration& a) { return gpForce(gpValueOf(m) * gpValueOf(a)); }

/// F = a * m
inline gpForce operator *(const gpLinearAcceleration& a, gpMass m) { return m * a; }

/// a = F / m
inline gpLinearAcceleration operator /(const gpForce& f, gpMass m)
{
    return gpLinearAcceleration(gpValueOf(f) / gpValueOf(m));
}

/// a = v / t
inline gpLinearAcceleration operator /(const gpLinearVelocity& v, gpTime t)
{
    return gpLinearAcceleration(gpValueOf(v) / (gpScalar)gpValueOf(t));
}

// Rotational Kinematics
//////////////////////////////////////////////////////////////////////////

/// A = w * dt
inline gpOrientation operator *(const gpAngularVelocity& w, gpTime dt)
{
    return gpOrientation(gpValueOf(w) * (gpScalar)gpValueOf(dt));
}

/// w = a * dt
inline gpAngularVelocity operator *(const gpAngularAcceleration& a, gpTime dt)
{
    return gpAngularVelocity(gpValueOf(a) * (gpScalar)gpValueOf(dt));
}

/// w = L * inverse(I)
inline gpAngularVelocity operator *(const gpInverseInertia& invI, const gpAngularMomentum& L)
{
    return gpAngularVelocity(gpValueOf(invI) * gpValueOf(L));
}

/// a = t * inverse(I)
inline gpAngularAcceleration operator *(const gpInverseInertia& invI, const gpTorque& t)
{
    return gpAngularAcceleration(gpValueOf(invI) * gpValueOf(t));
}

/// L = t * dt
inline gpAngularMomentum operator *(const gpTorque& t, gpTime dt)
{
    return gpAngularMomentum(gpValueOf(t) * (gpScalar)gpValueOf(dt));
}

// General Mat3
//////////////////////////////////////////////////////////////////////////

inline void gpOrthogonalize(gpMat3& m)
{
    // Use first column as reference.
    auto first = m.GetColumn(0);

    // Use second column to calculate third column.
    auto second = m.GetColumn(1);

    // Normalize the two reference vectors, otherwise it will explode!
    first.Normalize();
    second.Normalize();

    // Define third column by crossing the first with the second column.
    // \note Crossing normalized vectors yields another normalized vector.
    auto third = first.Cross(second);

    // Re-calculate the second column using the new third column and the first
    second = third.Cross(first);

    // Update the matrix.
    m.SetColumn(0, first);
    m.SetColumn(1, second);
    m.SetColumn(2, third);
}

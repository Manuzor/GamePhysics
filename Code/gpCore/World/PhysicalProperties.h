#pragma once

struct gpPhysicalProperties
{
    gpVec3 m_Position = { 0, 0, 0 };
    gpVec3 m_LinearVelocity = { 0, 0, 0 };
    gpScalar m_fGravityFactor = 1.0f;
    gpScalar m_fMass = 1.0f;
};

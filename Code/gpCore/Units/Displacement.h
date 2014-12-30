#pragma once

class DisplacementUnit
{
    gpVec3 m_Value;

    explicit DisplacementUnit(const gpVec3& Value) : m_Value(Value) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE friend
    DisplacementUnit Displacement(const gpVec3& Value) { return DisplacementUnit{ Value }; }

    EZ_FORCE_INLINE friend
    const gpVec3& ValueOf(const DisplacementUnit& d) { return d.m_Value; }
};

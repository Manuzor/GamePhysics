#pragma once

class gpDisplacementUnit
{
    gpVec3 m_Value;

    explicit gpDisplacementUnit(const gpVec3& Value) : m_Value(Value) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE friend
    gpDisplacementUnit gpDisplacement(const gpVec3& Value) { return gpDisplacementUnit{ Value }; }

    EZ_FORCE_INLINE friend       gpVec3& gpValueOf(      gpDisplacementUnit& d) { return d.m_Value; }
    EZ_FORCE_INLINE friend const gpVec3& gpValueOf(const gpDisplacementUnit& d) { return d.m_Value; }

public:
    gpDisplacementUnit() {}
};

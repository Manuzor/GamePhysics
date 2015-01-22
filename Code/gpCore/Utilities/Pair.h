#pragma once

template<typename First, typename Second>
class gpPair
{
private: // Data
    First first;
    Second second;

public: // Construction
    gpPair(const First& first, const Second& second) : first(first), second(second) {}

    gpPair(const gpPair& other)         = default;

public: // Accessors
    EZ_FORCE_INLINE friend First& gpFirstOf( gpPair& p) { return p.first; }
    EZ_FORCE_INLINE friend const First& gpFirstOf( const gpPair& p) { return p.first; }
    EZ_FORCE_INLINE friend First& gpSecondOf(gpPair& p) { return p.second; }
    EZ_FORCE_INLINE friend const First& gpSecondOf(const gpPair& p) { return p.second; }
};

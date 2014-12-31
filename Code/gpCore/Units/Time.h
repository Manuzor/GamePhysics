#pragma once

using gpTimeUnit = ezTime;

inline gpTimeUnit gpTime(double Value) { return gpTimeUnit::Seconds(Value); }

inline double gpValueOf(gpTimeUnit t) { return t.GetSeconds(); }

inline gpTimeUnit operator*(gpTimeUnit lhs, gpTimeUnit rhs) { return gpTimeUnit::Seconds(lhs.GetSeconds() * rhs.GetSeconds()); }
inline gpTimeUnit operator/(gpTimeUnit lhs, gpTimeUnit rhs) { return gpTimeUnit::Seconds(lhs.GetSeconds() / rhs.GetSeconds()); }

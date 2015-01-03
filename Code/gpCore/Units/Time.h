#pragma once

using gpTime = ezTime;

inline gpTime gpTime(double Value) { return gpTime::Seconds(Value); }

inline double gpValueOf(gpTime t) { return t.GetSeconds(); }

EZ_FORCE_INLINE gpTime operator *(gpTime lhs, gpTime rhs) { return gpTime::Seconds(lhs.GetSeconds() * rhs.GetSeconds()); }
EZ_FORCE_INLINE gpTime operator *(const gpTime& d, gpScalar factor) { return gpTime(gpValueOf(d) * factor); }
EZ_FORCE_INLINE gpTime operator *(gpScalar factor, const gpTime& d) { return d * factor; }
EZ_FORCE_INLINE gpTime operator /(gpTime lhs, gpTime rhs) { return gpTime::Seconds(lhs.GetSeconds() / rhs.GetSeconds()); }
EZ_FORCE_INLINE gpTime operator /(const gpTime& d, gpScalar factor) { return d * ezMath::Invert(factor); }

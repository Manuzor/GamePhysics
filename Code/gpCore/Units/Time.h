#pragma once

using TimeUnit = ezTime;

inline TimeUnit Time(double Value) { return ezTime::Seconds(Value); }

inline double ValueOf(ezTime t) { return t.GetSeconds(); }

inline ezTime operator*(ezTime lhs, ezTime rhs) { return ezTime::Seconds(lhs.GetSeconds() * rhs.GetSeconds()); }
inline ezTime operator/(ezTime lhs, ezTime rhs) { return ezTime::Seconds(lhs.GetSeconds() / rhs.GetSeconds()); }

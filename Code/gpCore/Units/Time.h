#pragma once

using gpTimeUnit = ezTime;

inline gpTimeUnit gpTime(double Value) { return gpTimeUnit::Seconds(Value); }

inline double gpValueOf(gpTimeUnit t) { return t.GetSeconds(); }

#pragma once

namespace ezMath
{
    template<typename Type>
    EZ_FORCE_INLINE bool IsEqual(Type lhs, Type rhs)
    {
        return IsEqual(lhs, rhs, BasicType<Type>::DefaultEpsilon());
    }

    template<typename Type>
    EZ_FORCE_INLINE bool IsZero(Type number)
    {
        return IsZero(number, BasicType<Type>::DefaultEpsilon());
    }
}

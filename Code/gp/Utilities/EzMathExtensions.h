#pragma once

namespace ezMath
{
    template<typename Type>
    EZ_FORCE_INLINE bool IsEqual(Type lhs, Type rhs)
    {
        return IsEqual(lhs, rhs, BasicType<Type>::DefaultEpsilon());
    }
}

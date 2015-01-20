#pragma once

using std::move;

template<typename CharacterType = char>
using gpStringData = const CharacterType*;

template<typename CharacterType = char>
using gpStringBuffer = CharacterType*;

#define GP_CheckDerefForNull EZ_OFF
#if EZ_ENABLED(GP_CheckDerefForNull)
// Inline functions to keep the syntax clean.
template<typename T> EZ_FORCE_INLINE       T* AddressOf(      T& x) { return &x; }
template<typename T> EZ_FORCE_INLINE const T* AddressOf(const T& x) { return &x; }

template<typename T> EZ_FORCE_INLINE       T& Deref(      T* x) { EZ_ASSERT(x, "Dereferencing nullptr!"); return *x; }
template<typename T> EZ_FORCE_INLINE const T& Deref(const T* x) { EZ_ASSERT(x, "Dereferencing nullptr!"); return *x; }
#else

#define AddressOf(x) (&x)
#define Deref(x) (*x)

#endif

using gpByte = unsigned char;

using gpScalar = float;

using gpVec2       = ezVec2Template<gpScalar>;
using gpVec3       = ezVec3Template<gpScalar>;
using gpRectI      = ezRectTemplate<ezInt32>;
using gpRectF      = ezRectTemplate<gpScalar>;
using gpQuaternion = ezQuatTemplate<gpScalar>;
using gpMat3       = ezMat3Template<gpScalar>;
using gpMat4       = ezMat4Template<gpScalar>;

EZ_FORCE_INLINE bool gpAreEqual(const gpVec2& lhs, const gpVec2& rhs, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return lhs.IsEqual(rhs, epsilon); }
EZ_FORCE_INLINE bool gpAreEqual(const gpVec3& lhs, const gpVec3& rhs, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return lhs.IsEqual(rhs, epsilon); }
EZ_FORCE_INLINE bool gpAreEqual(const gpMat3& lhs, const gpMat3& rhs, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return lhs.IsEqual(rhs, epsilon); }
EZ_FORCE_INLINE bool gpAreEqual(const gpMat4& lhs, const gpMat4& rhs, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return lhs.IsEqual(rhs, epsilon); }

/// \brief General AreEqual function
template<typename Type> EZ_FORCE_INLINE
bool gpAreEqual(Type lhs, Type rhs, Type epsilon) { return ezMath::IsEqual(lhs, rhs, epsilon); }

/// \brief Default ValueOf-function that returns the argument itself.
template<typename Type> EZ_FORCE_INLINE
Type& gpValueOf(Type& t) { return t; }

/// \brief Default ValueOf-function that returns the argument itself.
template<typename Type> EZ_FORCE_INLINE
const Type& gpValueOf(const Type& t) { return t; }

EZ_FORCE_INLINE ezResult gpInvert(gpMat3& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.Invert(epsilon); }
EZ_FORCE_INLINE ezResult gpInvert(gpMat4& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.Invert(epsilon); }

/// \brief General Invert function inverts the value fo the argument \a Value.
template<typename Type> EZ_FORCE_INLINE
void gpInvert(Type& Value) { gpValueOf(Value) = ezMath::Invert(gpValueOf(Value)); }

EZ_FORCE_INLINE const gpMat3 gpInverseOf(const gpMat3& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.GetInverse(); }
EZ_FORCE_INLINE const gpMat4 gpInverseOf(const gpMat4& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.GetInverse(); }

template<typename Type> EZ_FORCE_INLINE
Type gpInverseOf(Type Value) { return ezMath::Invert(gpValueOf(Value)); }

EZ_FORCE_INLINE void gpTranspose(gpMat3& m) { m.Transpose(); }
EZ_FORCE_INLINE void gpTranspose(gpMat4& m) { m.Transpose(); }
EZ_FORCE_INLINE const gpMat3 gpTransposeOf(const gpMat3& m) { return m.GetTranspose(); }
EZ_FORCE_INLINE const gpMat4 gpTransposeOf(const gpMat4& m) { return m.GetTranspose(); }

EZ_FORCE_INLINE bool gpIsZero(const gpVec2& v, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return v.IsZero(epsilon); }
EZ_FORCE_INLINE bool gpIsZero(const gpVec3& v, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return v.IsZero(epsilon); }
EZ_FORCE_INLINE bool gpIsZero(const gpMat3& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.IsZero(epsilon); }
EZ_FORCE_INLINE bool gpIsZero(const gpMat4& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.IsZero(epsilon); }

template<typename Type> EZ_FORCE_INLINE
bool gpIsZero(Type value, Type epsilon = ezMath::BasicType<Type>::SmallEpsilon())
{
    return ezMath::IsZero(value, epsilon);
}

EZ_FORCE_INLINE bool gpIsIdentity(const gpMat3& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.IsIdentity(epsilon); }
EZ_FORCE_INLINE bool gpIsIdentity(const gpMat4& m, gpScalar epsilon = ezMath::BasicType<gpScalar>::DefaultEpsilon()) { return m.IsIdentity(epsilon); }

EZ_FORCE_INLINE void gpAsArray(const gpMat3& m, gpScalar* out_pData, ezMatrixLayout::Enum layout) { m.GetAsArray(out_pData, layout); }
EZ_FORCE_INLINE void gpAsArray(const gpMat4& m, gpScalar* out_pData, ezMatrixLayout::Enum layout) { m.GetAsArray(out_pData, layout); }

enum class gpObjectInitialization { Yes, No };

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

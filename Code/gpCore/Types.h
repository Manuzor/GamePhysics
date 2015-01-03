#pragma once

using std::move;

template<typename CharacterType = char>
using gpStringData = const CharacterType*;

template<typename CharacterType = char>
using gpStringBuffer = CharacterType*;

#if EZ_ENABLED(EZ_OFF)
// Inline functions to keep the syntax clean.
template<typename T> EZ_FORCE_INLINE       T* AddressOf(      T& x) { return &x; }
template<typename T> EZ_FORCE_INLINE const T* AddressOf(const T& x) { return &x; }

template<typename T> EZ_FORCE_INLINE       T& Deref(      T* x) { EZ_ASSERT(x, ""); return *x; }
template<typename T> EZ_FORCE_INLINE const T& Deref(const T* x) { EZ_ASSERT(x, ""); return *x; }
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

EZ_FORCE_INLINE bool gpAreEqual(const gpVec2& lhs, const gpVec2& rhs, gpScalar epsilon) { return lhs.IsEqual(rhs, epsilon); }
EZ_FORCE_INLINE bool gpAreEqual(const gpVec3& lhs, const gpVec3& rhs, gpScalar epsilon) { return lhs.IsEqual(rhs, epsilon); }
EZ_FORCE_INLINE bool gpAreEqual(const gpMat3& lhs, const gpMat3& rhs, gpScalar epsilon) { return lhs.IsEqual(rhs, epsilon); }
EZ_FORCE_INLINE bool gpAreEqual(const gpMat4& lhs, const gpMat4& rhs, gpScalar epsilon) { return lhs.IsEqual(rhs, epsilon); }

/// \brief General AreEqual function
template<typename Type> EZ_FORCE_INLINE
bool gpAreEqual(Type lhs, Type rhs, Type epsilon) { return ezMath::IsEqual(lhs, rhs, epsilon); }

/// \brief Default ValueOf-function that returns the argument itself.
template<typename Type> EZ_FORCE_INLINE
Type& gpValueOf(Type& t) { return t; }

/// \brief Default ValueOf-function that returns the argument itself.
template<typename Type> EZ_FORCE_INLINE
const Type& gpValueOf(const Type& t) { return t; }

/// \brief General Invert function inverts the value fo the argument \a Value.
template<typename Type> EZ_FORCE_INLINE
Type gpInvert(Type Value) { return ezMath::Invert(gpValueOf(Value)); }

enum class gpObjectInitialization { Yes, No };

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

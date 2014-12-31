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
using gpRectI      = ezRectTemplate<ezUInt32>;
using gpRectF      = ezRectTemplate<gpScalar>;
using gpQuaternion = ezQuatTemplate<gpScalar>;
using gpMat3       = ezMat3Template<gpScalar>;
using gpMat4       = ezMat4Template<gpScalar>;
using gpTransform  = ezTransformTemplate<gpScalar>;

#define GP_DefineGlobalEzMathTypeEqualityFunction(TheTypeName)                                                      \
EZ_FORCE_INLINE bool gpAreEqual(const TheTypeName& lhs, const TheTypeName& rhs, TheTypeName::ComponentType epsilon) \
{                                                                                                                   \
    return lhs.IsEqual(rhs, epsilon);                                                                               \
}

GP_DefineGlobalEzMathTypeEqualityFunction(gpVec2)
GP_DefineGlobalEzMathTypeEqualityFunction(gpVec3)
GP_DefineGlobalEzMathTypeEqualityFunction(gpMat3)
GP_DefineGlobalEzMathTypeEqualityFunction(gpMat4)

#undef GP_DefineGlobalEzMathTypeEqualityFunction

template<typename Type> EZ_FORCE_INLINE
bool gpAreEqual(Type lhs, Type rhs, Type epsilon) { return ezMath::IsEqual(lhs, rhs, epsilon); }

template<typename Type> EZ_FORCE_INLINE
Type gpInvert(Type Value) { return ezMath::Invert(Value); }

enum class gpObjectInitialization { Yes, No };

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

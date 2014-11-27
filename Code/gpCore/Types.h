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

using gpVec2 = ezVec2Template<gpScalar>;
using gpVec3 = ezVec3Template<gpScalar>;
using gpRectI = ezRectTemplate<ezUInt32>;
using gpRectF = ezRectTemplate<gpScalar>;
using gpQuaternion = ezQuatTemplate<gpScalar>;
using gpMat3 = ezMat3Template<gpScalar>;
using gpMat4 = ezMat4Template<gpScalar>;
using gpTransform = ezTransformTemplate<gpScalar>;

enum class gpObjectInitialization { Yes, No };

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

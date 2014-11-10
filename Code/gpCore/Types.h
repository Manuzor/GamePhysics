#pragma once

using std::move;

template<typename CharacterType = char>
using gpStringData = const CharacterType*;

template<typename CharacterType = char>
using gpStringBuffer = CharacterType*;

#define AddressOf(x) (&x)
#define Deref(x) (*x)

using gpByte = unsigned char;

using gpScalar = float;

using gpVec2 = ezVec2Template<gpScalar>;
using gpVec3 = ezVec3Template<gpScalar>;
using gpRectI = ezRectTemplate<ezUInt32>;
using gpRectF = ezRectTemplate<gpScalar>;
using gpMat3 = ezMat3Template<gpScalar>;
using gpMat4 = ezMat4Template<gpScalar>;

enum class gpObjectInitialization { Yes, No };

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

class gpStaticClass
{
    gpStaticClass() = delete;
    ~gpStaticClass() = delete;
    gpStaticClass(gpStaticClass&&) = delete;
    gpStaticClass(const gpStaticClass&) = delete;
    void operator=(const gpStaticClass&) = delete;
};

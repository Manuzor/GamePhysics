#pragma once

using std::move;

template<typename CharacterType = char>
using gpStringData = const CharacterType*;

template<typename CharacterType = char>
using gpStringBuffer = CharacterType*;

#define AddressOf(x) (&x)
#define Deref(x) (*x)

using gpScalar = float;

using gpVec3 = ezVec3Template<gpScalar>;
using gpRect = ezRectTemplate<gpScalar>;
using gpMat3 = ezMat3Template<gpScalar>;
using gpMat4 = ezMat4Template<gpScalar>;

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

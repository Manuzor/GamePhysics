#pragma once

using std::move;

template<typename CharacterType = char>
using gpStringData = const CharacterType*;

template<typename CharacterType = char>
using gpStringBuffer = CharacterType*;

#define AddressOf(x) (&x)
#define Deref(x) (*x)

using gpScalar = float;

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

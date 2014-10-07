#pragma once

template<typename T>
using Ptr = T*;

template<typename T>
using Ref = T&;

template<typename T>
using Const = T const;

template<typename T>
using Volatile = T volatile;

template<typename CharacterType = char>
using StringData = Ptr<Const<CharacterType>>;

template<typename CharacterType = char>
using StringBuffer = Ptr<CharacterType>;

#define AddressOf(x) (&x)
#define Deref(x) (*x)

using Scalar = float;

#pragma once

using std::move;

template<typename T>
using Const = T const;

template<typename T>
using Ptr = T*;

template<typename T>
using ConstPtr = Const<Ptr<T>>;

template<typename T>
using Ref = T&;

template<typename T>
using ConstRef = Const<Ref<T>>;

template<typename T>
using Volatile = T volatile;

template<typename CharacterType = char>
using StringData = Ptr<Const<CharacterType>>;

template<typename CharacterType = char>
using StringBuffer = Ptr<CharacterType>;

#define AddressOf(x) (&x)
#define Deref(x) (*x)

using Scalar = float;

class gpNotImplementedException : public std::exception
{
    const char* what() const override { return "Not implemented."; }
};

#pragma once

namespace gpInternal
{
    struct gpGetConstViewHelper
    {
        template<typename T> struct GetConst                 { using Type = const T; };
        template<typename T> struct GetConst<T*>             { using Type = const T*; };
        template<typename T> struct GetConst<const T*>       { using Type = const T*; };
        template<typename T> struct GetConst<T* const>       { using Type = const T* const; };
        template<typename T> struct GetConst<const T* const> { using Type = const T* const; };
    };
}

template<typename Type, typename ReturnType = ezArrayPtr<Type>>
EZ_FORCE_INLINE ReturnType gpGetView(ezDynamicArray<Type>& Array)
{
    return ReturnType(Array.GetData(), Array.GetCount());
}

template<typename Type, typename ReturnType = ezArrayPtr<typename gpInternal::gpGetConstViewHelper::GetConst<Type>::Type>>
EZ_FORCE_INLINE ReturnType gpGetConstView(ezDynamicArray<Type>& Array)
{
    return ReturnType(const_cast<typename gpInternal::gpGetConstViewHelper::GetConst<Type>::Type*>(Array.GetData()), Array.GetCount());
}

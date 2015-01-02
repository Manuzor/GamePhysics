#pragma once

#include "gpCore/Config.h"
#include "gpCore/Types.h"

#ifdef GP_CoreExport
#define GP_CoreAPI __declspec(dllexport)
#else
#define GP_CoreAPI __declspec(dllimport)
#endif

#define GP_NotImplemented do {             \
    EZ_REPORT_FAILURE("Not implemented."); \
    throw gpNotImplementedException();     \
} while(false)

namespace gpInternal
{
    template<typename CallableType>
    struct ScopeExitHelper
    {
        CallableType m_ToCall;

        ScopeExitHelper(CallableType ToCall) : m_ToCall(ToCall) {}
        ~ScopeExitHelper() { EZ_ASSERT(bool(m_ToCall), ""); m_ToCall(); }

        ScopeExitHelper(const ScopeExitHelper&) = delete;
        void operator=(const ScopeExitHelper&) = delete;
    };
}

template<typename Type> EZ_FORCE_INLINE void gpAddReferenceTo(Type& object)  { object.AddRef(); }
template<typename Type> EZ_FORCE_INLINE void gpAddReferenceTo(Type* pObject) { pObject->AddRef(); }

template<typename Type> EZ_FORCE_INLINE void gpReleaseReferenceTo(Type& object)  { object.ReleaseRef(); }
template<typename Type> EZ_FORCE_INLINE void gpReleaseReferenceTo(Type* pObject) { pObject->ReleaseRef(); }

/// \brief Dereferences the \a this pointer in a more human-friendly way.
///
/// Helps to keep the code clean of asterisks
#define self (*this)

#define GP_OnScopeExit gpInternal::ScopeExitHelper<std::function<void()>> EZ_CONCAT(ScopeExit_, EZ_SOURCE_LINE) = [&]

namespace gpInternal
{
    template<typename T> struct ValueTypeOf { using Type = T; };
}

/// \brief Type function to get the value type of a given type \a Type.
template<typename Type>
using ValueTypeOf = typename gpInternal::ValueTypeOf<Type>::Type;

#define GP_DeclareValueTypeFunction(TheContainingType, TheValueType) namespace gpInternal \
{                                                                                         \
    template<> struct ValueTypeOf<TheContainingType> { using Type = TheValueType; };      \
}

#include "gpCore/Units.h"
#include "gpCore/Transform.h"

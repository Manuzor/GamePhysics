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

#define GP_OnScopeExit gpInternal::ScopeExitHelper<std::function<void()>> EZ_CONCAT(ScopeExit_, EZ_SOURCE_LINE) = [&]

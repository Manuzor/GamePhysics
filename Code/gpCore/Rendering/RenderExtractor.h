#pragma once
#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/AsyncRendering.h"
#include "gpCore/Memory/FixedStackAllocator.h"

class gpRenderer;
class gpRenderExtractor;

using gpRenderExtractionEvent = ezEvent<gpRenderExtractor*>;
using gpRenderExtractionListener = gpRenderExtractionEvent::Handler;

class GP_CoreAPI gpRenderExtractor
{
    friend gpRenderer;

// Non-static
//////////////////////////////////////////////////////////////////////////

public:
    gpRenderExtractor(gpRenderExtractor&) = delete;
    void operator=(gpRenderExtractor&) = delete;

    template<typename Type>
    Type* AllocateRenderData(gpObjectInitialization init = gpObjectInitialization::Yes)
    {
        auto pResult = (Type*)s_pInstance->AllocateRenderData(sizeof(Type));
        if(init == gpObjectInitialization::Yes)
            new (pResult) Type(); // Default construct it.
        return pResult;
    }

private:
    gpRenderExtractor();
    ~gpRenderExtractor() { ClearData(); }

    void ClearData();
    gpDrawData::Base* AllocateRenderData(size_t uiSize);

private:

    EZ_FORCE_INLINE void SwapDataPointers() { std::swap(m_pActiveRenderData, m_pCollectedRenderData); }

private:
    gpRenderExtractionEvent m_ExtractionEvent;

    gpFixedStackAllocator m_renderDataAllocators[2];

    /// The data used by the renderer. Beware of asynchronous access.
    gpFixedStackAllocator* m_pActiveRenderData;

    /// The collected data that will be rendered with the next swap.
    gpFixedStackAllocator* m_pCollectedRenderData;

    bool m_bIsExtracting;

// Static
//////////////////////////////////////////////////////////////////////////

public:
    static void Initialize();
    static void Shutdown();

    static void AddExtractionListener(gpRenderExtractionListener listener)    { EZ_ASSERT(IsInitialized(), ""); s_pInstance->m_ExtractionEvent.AddEventHandler(listener); }
    static void RemoveExtractionListener(gpRenderExtractionListener listener) { EZ_ASSERT(IsInitialized(), ""); s_pInstance->m_ExtractionEvent.RemoveEventHandler(listener); }

    static void TriggerExtraction();

private:
    EZ_FORCE_INLINE static bool IsInitialized() { return s_pInstance != nullptr; }

    static void SwapBuffers();

private:
    static gpRenderExtractor* s_pInstance;

    EZ_MAKE_SUBSYSTEM_STARTUP_FRIEND(GamePhysics, RenderExtractor);
};

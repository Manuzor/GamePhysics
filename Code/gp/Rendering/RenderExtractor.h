#pragma once
#include "gp/Rendering/Rendering.h"
#include "gp/Memory/FixedStackAllocator.h"

class gpRenderer;
class gpRenderExtractor;

using gpRenderExtractionEvent = ezEvent<gpRenderExtractor*>;
using gpRenderExtractionListener = gpRenderExtractionEvent::Handler;

class gpRenderExtractor
{
    friend gpRenderer;

public:
    static void AddExtractionListener(gpRenderExtractionListener listener)    { s_ExtractionEvent.AddEventHandler(listener); }
    static void RemoveExtractionListener(gpRenderExtractionListener listener) { s_ExtractionEvent.RemoveEventHandler(listener); }

private:
    static gpRenderExtractionEvent s_ExtractionEvent;

public:
    gpRenderExtractor(gpRenderExtractor&) = delete;
    void operator=(gpRenderExtractor&) = delete;

    gpRenderExtractor();
    ~gpRenderExtractor() { ClearData(); }

    void TriggerExtraction();

    template<typename Type>
    Type* AllocateRenderData(gpObjectInitialization init = gpObjectInitialization::Yes)
    {
        auto pResult = (Type*)AllocateRenderData(sizeof(Type));
        if(init == gpObjectInitialization::Yes)
            new (pResult) Type(); // Default construct it.
        return pResult;
    }

private:
    void ClearData();

    gpDrawData::Base* AllocateRenderData(size_t uiSize);

private:

    EZ_FORCE_INLINE void SwapDataPointers() { std::swap(m_pActiveRenderData, m_pCollectedRenderData); }

private:
    gpFixedStackAllocator m_renderDataAllocators[2];

    /// The data used by the renderer. Beware of asynchronous access.
    gpFixedStackAllocator* m_pActiveRenderData;

    /// The collected data that will be rendered with the next swap.
    gpFixedStackAllocator* m_pCollectedRenderData;
};

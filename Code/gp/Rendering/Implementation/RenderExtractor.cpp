#include "gp/PCH.h"
#include "gp/Rendering/RenderExtractor.h"

gpRenderExtractionEvent gpRenderExtractor::s_ExtractionEvent;

void gpRenderExtractor::TriggerExtraction()
{
    s_ExtractionEvent.Broadcast(this);

    // TODO Wait for render thread signal.
    SwapDataPointers();
    // TODO Signal render thread.
    m_pCollectedRenderData->Reset();
}

gpRenderExtractor::gpRenderExtractor() :
    m_pActiveRenderData(AddressOf(m_renderDataAllocators[0])),
    m_pCollectedRenderData(AddressOf(m_renderDataAllocators[1]))
{
    for (auto& allocator : m_renderDataAllocators)
    {
        allocator.Resize(1024 * 1024);
    }
}

void gpRenderExtractor::ClearData()
{
    for(auto& allocator : m_renderDataAllocators)
    {
        allocator.Clear();
    }
}

gpDrawData::Base* gpRenderExtractor::AllocateRenderData(size_t uiSize)
{
    return (gpDrawData::Base*)m_pCollectedRenderData->Allocate(uiSize);
}

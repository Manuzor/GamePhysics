#include "gp/PCH.h"
#include "gp/Rendering/AsyncRendering.h"
#include "gp/Rendering/RenderExtractor.h"
#include "gp/Rendering/Renderer.h"

gpRenderExtractor* gpRenderExtractor::s_pInstance = nullptr;

EZ_BEGIN_SUBSYSTEM_DECLARATION(GamePhysics, RenderExtractor)
    BEGIN_SUBSYSTEM_DEPENDENCIES
        "Renderer"
    END_SUBSYSTEM_DEPENDENCIES

    ON_ENGINE_STARTUP
    {
        gpRenderExtractor::Initialize();
    }

    ON_ENGINE_SHUTDOWN
    {
        gpRenderExtractor::Shutdown();
    }

EZ_END_SUBSYSTEM_DECLARATION

void gpRenderExtractor::Initialize()
{
    EZ_ASSERT(ezThreadUtils::IsMainThread(), "Can only initialize render extractor from main thread!");

    if (IsInitialized())
        return;

    static gpByte staticInstanceData[sizeof(gpRenderExtractor)];

    s_pInstance = new (staticInstanceData) gpRenderExtractor();

    gpRenderer::SetExtractor(s_pInstance);
}

void gpRenderExtractor::Shutdown()
{
    EZ_ASSERT(ezThreadUtils::IsMainThread(), "Can only shutdown render extractor from main thread!");

    if (!IsInitialized())
        return;

    s_pInstance->~gpRenderExtractor();
    s_pInstance = nullptr;
}

void gpRenderExtractor::TriggerExtraction()
{
    EZ_ASSERT(!s_pInstance->m_bIsExtracting, "Cannot trigger an extraction while extracting...");
    {
        s_pInstance->m_bIsExtracting = true;
        GP_OnScopeExit{ s_pInstance->m_bIsExtracting = false; };

        s_pInstance->m_ExtractionEvent.Broadcast(s_pInstance);
    }

    SwapBuffers();

    s_pInstance->m_pCollectedRenderData->Reset();
}

void gpRenderExtractor::SwapBuffers()
{
#if EZ_ENABLED(GP_AsyncRendering)
    gpAsyncRendering::GetExtractionSignal()->WaitForSignal();
    s_pInstance->SwapDataPointers();
    gpAsyncRendering::GetRenderSignal()->RaiseSignal();
#else
    s_pInstance->SwapDataPointers();
    gpRenderer::Render();
#endif
}

// Non-static
//////////////////////////////////////////////////////////////////////////

gpRenderExtractor::gpRenderExtractor() :
    m_pActiveRenderData(AddressOf(m_renderDataAllocators[0])),
    m_pCollectedRenderData(AddressOf(m_renderDataAllocators[1])),
    m_bIsExtracting(false)
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
    EZ_ASSERT(m_bIsExtracting, "Can only allocate render data while extracting!");
    return (gpDrawData::Base*)m_pCollectedRenderData->Allocate(uiSize);
}

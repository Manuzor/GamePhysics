#include "gp/PCH.h"
#include "gp/Rendering/Renderer.h"
#include "gp/Rendering/RenderExtractor.h"
#include "gp/Window.h"

bool gpRenderer::s_bInitialized = false;
gpRenderExtractor* gpRenderer::s_pExtractor = nullptr;
ezSizeU32 gpRenderer::s_Resolution;

EZ_BEGIN_SUBSYSTEM_DECLARATION(GamePhysics, Renderer)
    ON_ENGINE_STARTUP
    {
        gpRenderer::Initialize();
    }

    ON_ENGINE_SHUTDOWN
    {
        gpRenderer::Shutdown();
    }
EZ_END_SUBSYSTEM_DECLARATION

gpFixedStackAllocator* gpRenderer::GetRenderData()
{
    EZ_ASSERT(s_pExtractor, "No extractor set!");
    return s_pExtractor->m_pActiveRenderData;
}

static void DrawHelper(gpDrawData::Base* pDrawData)
{
    switch(pDrawData->m_Type)
    {
    case gpDrawData::Type::Point:   gpDraw(*static_cast<gpDrawData::Point*  >(pDrawData)); return;
    case gpDrawData::Type::Line:    gpDraw(*static_cast<gpDrawData::Line*   >(pDrawData)); return;
    case gpDrawData::Type::Arrow:   gpDraw(*static_cast<gpDrawData::Arrow*  >(pDrawData)); return;
    case gpDrawData::Type::Polygon: gpDraw(*static_cast<gpDrawData::Polygon*>(pDrawData)); return;
    case gpDrawData::Type::Circle:  gpDraw(*static_cast<gpDrawData::Circle* >(pDrawData)); return;
    case gpDrawData::Type::Box:     gpDraw(*static_cast<gpDrawData::Box*    >(pDrawData)); return;
    }
}

void gpRenderer::Render()
{
    glViewport(0, 0, s_Resolution.width, s_Resolution.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, s_Resolution.width, -(ezInt32)s_Resolution.height, 0, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_CULL_FACE);

    glClearColor(0, 0, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GP_OpenGLScope_EnableDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    auto pData = gpRenderer::GetRenderData();
    auto pCurrentData = (gpDrawData::Base*)pData->GetBegin();
    auto pEnd = (gpDrawData::Base*)pData->GetCurrent();
    while(pCurrentData < pEnd)
    {
        DrawHelper(pCurrentData);
        pCurrentData = pCurrentData->GetNext();
    }
}

static void WidthChangedCallback(const ezCVar::CVarEvent& Event)
{
    if (Event.m_EventType != ezCVar::CVarEvent::ValueChanged)
        return;

    auto Res = gpRenderer::GetResolution();
    Res.width = (decltype(Res.width))static_cast<ezCVarInt*>(Event.m_pCVar)->GetValue();
}

static void HeightChangedCallback(const ezCVar::CVarEvent& Event)
{
    if (Event.m_EventType != ezCVar::CVarEvent::ValueChanged)
        return;

    auto Res = gpRenderer::GetResolution();
    Res.height = (decltype(Res.height))static_cast<ezCVarInt*>(Event.m_pCVar)->GetValue();
}

#if EZ_ENABLED(GP_AsyncRendering)
#include "gp/Rendering/Implementation/Renderer_Async.inl"
#else
#include "gp/Rendering/Implementation/Renderer_NonAsync.inl"
#endif

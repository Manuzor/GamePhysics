#include "gpCore/PCH.h"
#include "gpCore/Rendering/Renderer.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/Window.h"

bool gpRenderer::s_bInitialized = false;
gpRenderExtractor* gpRenderer::s_pExtractor = nullptr;
ezSizeU32 gpRenderer::s_Resolution;

static ezProfilingId g_ProfilingId_Render;

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
    EZ_PROFILE(g_ProfilingId_Render);

    glViewport(0, 0, s_Resolution.width, s_Resolution.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, s_Resolution.width, s_Resolution.height, 0, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_CULL_FACE);

    glClearColor(0, 0, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GP_OpenGLScope_EnableDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto pData = (gpDrawData::Base*)gpRenderer::GetRenderData()->GetBegin();
    auto pEnd = (gpDrawData::Base*)gpRenderer::GetRenderData()->GetCurrent();
    while(pData < pEnd)
    {
        DrawHelper(pData);
        pData = gpDrawData::gpNext(pData);
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
#include "gpCore/Rendering/Implementation/Renderer_Async.inl"
#else
#include "gpCore/Rendering/Implementation/Renderer_NonAsync.inl"
#endif

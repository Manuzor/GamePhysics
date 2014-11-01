#include "gp/PCH.h"
#include "gp/Rendering/Renderer.h"
#include "gp/Rendering/RenderExtractor.h"
#include "gp/Window.h"

bool gpRenderer::s_bInitialized = false;
gpRenderExtractor* gpRenderer::s_pExtractor = nullptr;

#if EZ_ENABLED(GP_AsyncRendering)
gpInternal::gpRenderThread* gpRenderer::s_pRenderThread = nullptr;
#endif

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

void gpRenderer::Initialize()
{
    if (IsInitialized())
        return;

#if EZ_ENABLED(GP_AsyncRendering)
    static gpByte staticRenderThreadData[sizeof(gpInternal::gpRenderThread)];
    s_pRenderThread = new (staticRenderThreadData) gpInternal::gpRenderThread();
    s_pRenderThread->Start();
#endif

    s_bInitialized = true;
}

void gpRenderer::Shutdown()
{
    if (!IsInitialized())
        return;

#if EZ_ENABLED(GP_AsyncRendering)
    s_pRenderThread->Stop();
#endif

    s_bInitialized = false;
}

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
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(-20, 20, -20, 20, -10, 10);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

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

#if EZ_ENABLED(GP_AsyncRendering)
gpInternal::gpRenderThread::gpRenderThread() :
    ezThread("Render Thread"),
    m_bStop(false)
{
}

void gpInternal::gpRenderThread::Stop()
{
    m_bStop = true;
    gpAsyncRendering::GetRenderSignal()->RaiseSignal();
    ezLog::Info("Waiting for render thread...");
    Join();
    ezLog::Info("Render thread successfully terminated.");

    // TODO Make sure the main thread is not blocked?
    gpAsyncRendering::GetExtractionSignal()->RaiseSignal();
}

ezUInt32 gpInternal::gpRenderThread::Run()
{
    ezLog::Info("Render thread is running.");

    // Make sure the extractor is not waiting for us indefinitely.
    GP_OnScopeExit{ gpAsyncRendering::GetExtractionSignal()->RaiseSignal(); };

    while(true)
    {
        gpAsyncRendering::GetExtractionSignal()->RaiseSignal(); // "Hey extractor, continue extracting!"
        if(m_bStop)
            break;
        gpAsyncRendering::GetRenderSignal()->WaitForSignal(); // "[Lets wait for the extractor to give us data.]"
        if (m_bStop)
            break;

        gpRenderer::Render();
    }

    ezLog::Info("Render thread finished.");

    return 0;
}
#endif

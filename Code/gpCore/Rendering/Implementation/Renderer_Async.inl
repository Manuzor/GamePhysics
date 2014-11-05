#include "gpCore/Rendering/Implementation/RenderThread.h"

static gpInternal::gpRenderThread* s_pRenderThread = nullptr;

void gpRenderer::Initialize()
{
    if (IsInitialized())
        return;

    static gpByte staticRenderThreadData[sizeof(gpInternal::gpRenderThread)];
    s_pRenderThread = new (staticRenderThreadData) gpInternal::gpRenderThread();
    s_pRenderThread->Start();

    s_bInitialized = true;
}

void gpRenderer::Shutdown()
{
    if (!IsInitialized())
        return;

    s_pRenderThread->Stop();

    s_bInitialized = false;
}

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

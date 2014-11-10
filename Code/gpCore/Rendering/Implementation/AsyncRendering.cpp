#include "gpCore/PCH.h"
#include "gpCore/Rendering/AsyncRendering.h"

ezThreadSignal gpAsyncRendering::s_RenderSignal;
ezThreadSignal gpAsyncRendering::s_ExtractionSignal;

ezDelegate<ezResult()> gpAsyncRenderingStartup::s_InitializeOnRenderThread;
ezResult gpAsyncRenderingStartup::s_InitializationResult = EZ_FAILURE;
ezThreadSignal gpAsyncRenderingStartup::s_RenderThreadSignal;
ezThreadSignal gpAsyncRenderingStartup::s_MainThreadSignal;

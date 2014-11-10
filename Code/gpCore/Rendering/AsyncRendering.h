#pragma once

class gpAsyncRendering
{
    gpAsyncRendering() = delete;
    gpAsyncRendering(const gpAsyncRendering&) = delete;
    void operator=(const gpAsyncRendering&) = delete;

public:

    EZ_FORCE_INLINE static ezThreadSignal* GetRenderSignal() { return AddressOf(s_RenderSignal); }
    EZ_FORCE_INLINE static ezThreadSignal* GetExtractionSignal() { return AddressOf(s_ExtractionSignal); }

private:
    static ezThreadSignal s_RenderSignal;
    static ezThreadSignal s_ExtractionSignal;
};

namespace gpAsyncRenderingStartup
{
    extern GP_CoreAPI ezDelegate<ezResult()> s_InitializeOnRenderThread;
    extern GP_CoreAPI ezResult s_InitializationResult;
    extern GP_CoreAPI ezThreadSignal s_MainThreadSignal;
    extern GP_CoreAPI ezThreadSignal s_RenderThreadSignal;
};

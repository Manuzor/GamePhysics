#pragma once

#define GP_AsyncRendering EZ_OFF

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

#pragma once
#include "gp/Rendering/AsyncRendering.h"

class gpRenderExtractor;
class gpFixedStackAllocator;
class gpWindow;

class gpRenderer
{
    friend gpRenderExtractor;
public:
    gpRenderer() = delete;
    gpRenderer(const gpRenderer&) = delete;
    void operator=(const gpRenderer&) = delete;

    static void SetExtractor(gpRenderExtractor* pExtractor) { EZ_ASSERT(IsInitialized(), ""); s_pExtractor = pExtractor; }
    static gpRenderExtractor* GetExtractor() { EZ_ASSERT(IsInitialized(), ""); return s_pExtractor; }

    static void SetResolution(const ezSizeU32& newResolution) { s_Resolution = newResolution; }
    static ezSizeU32 GetResolution() { return s_Resolution; }

    static void Render();

private:
    static void Initialize();
    static void Shutdown();

    EZ_FORCE_INLINE static bool IsInitialized() { return s_bInitialized; }

    static gpFixedStackAllocator* GetRenderData();

private:
    static bool s_bInitialized;
    static gpRenderExtractor* s_pExtractor;
    static ezSizeU32 s_Resolution;

    EZ_MAKE_SUBSYSTEM_STARTUP_FRIEND(GamePhysics, Renderer);
};

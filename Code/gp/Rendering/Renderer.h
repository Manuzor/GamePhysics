#pragma once
#include "gp/Rendering/AsyncRendering.h"

class gpRenderExtractor;
class gpFixedStackAllocator;

class gpRenderer
{
    friend gpRenderExtractor;
public:
    gpRenderer() = delete;
    gpRenderer(const gpRenderer&) = delete;
    void operator=(const gpRenderer&) = delete;

    static void SetExtractor(gpRenderExtractor* pExtractor) { EZ_ASSERT(IsInitialized(), ""); s_pExtractor = pExtractor; }
    static gpRenderExtractor* GetExtractor() { EZ_ASSERT(IsInitialized(), ""); return s_pExtractor; }

    static void Render();

private:
    static void Initialize();
    static void Shutdown();

    EZ_FORCE_INLINE static bool IsInitialized() { return s_bInitialized; }

    static gpFixedStackAllocator* GetRenderData();

private:
    static bool s_bInitialized;
    static gpRenderExtractor* s_pExtractor;

    EZ_MAKE_SUBSYSTEM_STARTUP_FRIEND(GamePhysics, Renderer);
};

#pragma once
#include "gp/Rendering/AsyncRendering.h"

class gpRenderExtractor;
class gpFixedStackAllocator;

#if EZ_ENABLED(GP_AsyncRendering)
namespace gpInternal
{
    class gpRenderThread : public ezThread
    {
    public:
        gpRenderThread();
        virtual ezUInt32 Run() override;
        void Stop();
    private:
        bool m_bStop;
    };
}
#endif

class gpRenderer
{
    friend gpRenderExtractor;
public:
    gpRenderer() = delete;
    gpRenderer(const gpRenderer&) = delete;
    void operator=(const gpRenderer&) = delete;

    static void SetExtractor(gpRenderExtractor* pExtractor) { EZ_ASSERT(IsInitialized(), ""); s_pExtractor = pExtractor; }
    static gpRenderExtractor* GetExtractor() { EZ_ASSERT(IsInitialized(), ""); return s_pExtractor; }

private:
    static void Initialize();
    static void Shutdown();

    EZ_FORCE_INLINE static bool IsInitialized() { return s_bInitialized; }

    static gpFixedStackAllocator* GetRenderData();

    static void Render();

private:
    static bool s_bInitialized;
    static gpRenderExtractor* s_pExtractor;

#if EZ_ENABLED(GP_AsyncRendering)
    friend gpInternal::gpRenderThread;
    static gpInternal::gpRenderThread* s_pRenderThread;
#endif

    EZ_MAKE_SUBSYSTEM_STARTUP_FRIEND(GamePhysics, Renderer);
};

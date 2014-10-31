#pragma once

#if EZ_ENABLED(GP_UseOpenGL)
#include "gp/Rendering/OpenGL/OpenGL.h"
#elif EZ_ENABLED(GP_UseDirect3D)
#include "gp/Rendering/Direct3D/Direct3D.h"
#endif

class gpRenderer
{
public:
private:
};

class gpRenderExtractor
{
    friend gpRenderer;
public:
    struct Context
    {

    };

public:
    //using Event = ezEvent<ExtractionContext*>;
    //using Listener = Event::Handler;

private:


private:
    gpRenderExtractor() = delete;
    gpRenderExtractor(gpRenderExtractor&) = delete;
    void operator=(gpRenderExtractor&) = delete;
};

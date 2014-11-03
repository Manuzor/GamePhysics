#include "gp/PCH.h"
#include "gp/Application/ApplicationBase.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/Renderer.h"
#include "gp/Rendering/RenderExtractor.h"

void gpApplicationBase::RenderFrame()
{
    gpRenderExtractor::TriggerExtraction();

    gpDrawData::Line l;
    l.m_Start.Set(0.0f, 0.0f, 0.0f);
    l.m_End.Set(100.0f, -100.0f, 0.0f);

    gpDraw(l);
}

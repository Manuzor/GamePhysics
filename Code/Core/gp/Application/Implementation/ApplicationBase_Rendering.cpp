#include "gp/PCH.h"
#include "gp/Application/ApplicationBase.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/Renderer.h"
#include "gp/Rendering/RenderExtractor.h"

void gpApplicationBase::RenderFrame()
{
    gpRenderExtractor::TriggerExtraction();
}

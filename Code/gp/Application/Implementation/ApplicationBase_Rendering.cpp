#include "gp/PCH.h"
#include "gp/Application/ApplicationBase.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/Renderer.h"
#include "gp/Rendering/RenderExtractor.h"

void gpApplicationBase::RenderFrame()
{
    auto resolution = m_pWindow->GetResolution();
    glViewport(0, 0, resolution.width, resolution.height);

    gpRenderExtractor::TriggerExtraction();
}

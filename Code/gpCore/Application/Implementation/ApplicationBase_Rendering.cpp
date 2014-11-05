#include "gpCore/PCH.h"
#include "gpCore/Application/ApplicationBase.h"
#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/Renderer.h"
#include "gpCore/Rendering/RenderExtractor.h"

void gpApplicationBase::RenderFrame()
{
    gpRenderExtractor::TriggerExtraction();
}

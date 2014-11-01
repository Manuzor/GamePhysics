#include "gp/PCH.h"
#include "gp/Rendering/AsyncRendering.h"

ezThreadSignal gpAsyncRendering::s_RenderSignal;
ezThreadSignal gpAsyncRendering::s_ExtractionSignal;

#include "gp/PCH.h"
#include "gp/Rendering/Renderer.h"
#include "gp/Rendering/RenderExtractor.h"

static void DrawHelper(gpDrawData::Base* pDrawData)
{
    switch(pDrawData->m_Type)
    {
    case gpDrawData::Type::Point:   gpDraw(*static_cast<gpDrawData::Point*  >(pDrawData));
    case gpDrawData::Type::Line:    gpDraw(*static_cast<gpDrawData::Line*   >(pDrawData));
    case gpDrawData::Type::Arrow:   gpDraw(*static_cast<gpDrawData::Arrow*  >(pDrawData));
    case gpDrawData::Type::Polygon: gpDraw(*static_cast<gpDrawData::Polygon*>(pDrawData));
    case gpDrawData::Type::Circle:  gpDraw(*static_cast<gpDrawData::Circle* >(pDrawData));
    case gpDrawData::Type::Box:     gpDraw(*static_cast<gpDrawData::Box*    >(pDrawData));
    default:
        break;
    }
}

void gpRenderer::Render()
{
    for (ezUInt32 i = 0; i < m_extractors.GetCount(); ++i)
    {
        auto pExtractor = m_extractors[i];
        // TODO Wait for extractor signal.
        auto pData = pExtractor->m_pActiveRenderData;
        auto pCurrentData = (gpDrawData::Base*)pData->GetBegin();
        auto pEnd = (gpDrawData::Base*)pData->GetEnd();
        while(pCurrentData < pEnd)
        {
            DrawHelper(pCurrentData);
            pCurrentData = pCurrentData->GetNext();
        }
    }
}

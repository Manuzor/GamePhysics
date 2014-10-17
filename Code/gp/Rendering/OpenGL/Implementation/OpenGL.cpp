#include "gp/PCH.h"
#include "gp/Rendering/OpenGL/OpenGL.h"

using ezMath::Cos;
using ezMath::Sin;

void gpDraw(const gpDrawData::Point& Data)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glColor4fv(&Data.m_Color.r);
    glPointSize(Data.m_fPointSize);

    GP_OpenGLScope_BeginEnd(GL_POINTS);
    glVertex3fv(&Data.m_Position.x);
}

void gpDraw(const gpDrawData::Line& Data)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor4fv(&Data.m_Color.r);

    GP_OpenGLScope_BeginEnd(GL_LINES);
    glVertex3fv(&Data.m_Start.x);
    glVertex3fv(&Data.m_End.x);
}

void gpDraw(const gpDrawData::Polygon& Data)
{
    EZ_ASSERT(Data.m_bDrawFill || Data.m_bDrawOutline, "No color given to draw.");

    if (Data.m_bDrawFill)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor4fv(&Data.m_FillColor.r);
    }

    if (Data.m_bDrawOutline)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor4fv(&Data.m_OutlineColor.r);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GP_NotImplemented;
}

static void gpDrawCircleHelper(const gpDrawData::Circle& Data, GLenum PolygonMode, const ezColor& Color)
{
    glPolygonMode(GL_FRONT_AND_BACK, PolygonMode);
    glColor4fv(&Color.r);

    auto Angle = ezAngle::Degree(360.0f / Data.m_uiNumLineSegments);
    gpVec3 CurrentSegment;

    GP_OpenGLScope_BeginEnd(GL_POLYGON);
    for (decltype(Data.m_uiNumLineSegments) i = 0; i < Data.m_uiNumLineSegments; ++i)
    {
        CurrentSegment.Set(Cos(Angle * float(i)) * Data.m_fRadius,
                            Sin(Angle * float(i)) * Data.m_fRadius,
                            0.0f);
        auto Result = Data.m_Position + CurrentSegment;
        glVertex3fv(&Result.x);
    }
}

void gpDraw(const gpDrawData::Circle& Data)
{
    EZ_ASSERT(Data.m_bDrawFill || Data.m_bDrawOutline, "No color given to draw.");
    EZ_ASSERT(Data.m_uiNumLineSegments >= 3, "");

    if (Data.m_bDrawFill)
    {
        gpDrawCircleHelper(Data, GL_FILL, Data.m_FillColor);
    }

    if (Data.m_bDrawOutline)
    {
        gpDrawCircleHelper(Data, GL_LINE, Data.m_OutlineColor);
    }
}

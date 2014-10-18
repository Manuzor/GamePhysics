#include "gp/PCH.h"
#include "gp/Rendering/OpenGL/OpenGL.h"
#include "gp/Utilities/EzMathExtensions.h"

using ezMath::Cos;
using ezMath::Sin;
using ezMath::IsEqual;

void gpDraw(const gpDrawData::Point& Data)
{
    if (IsEqual(Data.m_Color.a, 0.0f))
        return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glColor4fv(&Data.m_Color.r);
    glPointSize(Data.m_fPointSize);

    GP_OpenGLScope_BeginEnd(GL_POINTS);
    glVertex3fv(&Data.m_Position.x);
}

void gpDraw(const gpDrawData::Line& Data)
{
    if (IsEqual(Data.m_Color.a, 0.0f))
        return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor4fv(&Data.m_Color.r);

    glLineWidth(Data.m_fLineWidth);

    GP_OpenGLScope_BeginEnd(GL_LINES);
    glVertex3fv(&Data.m_Start.x);
    glVertex3fv(&Data.m_End.x);
}

void gpDraw(const gpDrawData::Arrow& Data)
{
    if (IsEqual(Data.m_Color.a, 0.0f))
        return;

    auto line = Data.m_End - Data.m_Start;
    auto fScale = line.GetLength();
    line.Normalize();

    auto LeftWing = gpVec3(Cos(Data.m_WingAngle), Sin(Data.m_WingAngle), 0.0f) * Data.m_fWingScale;
    auto RightWing = gpVec3(Cos(Data.m_WingAngle), -Sin(Data.m_WingAngle), 0.0f) * Data.m_fWingScale;

    LeftWing = LeftWing + Data.m_End;
    RightWing = RightWing + Data.m_End;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor4fv(&Data.m_Color.r);

    glLineWidth(Data.m_fLineWidth);

    {
        GP_OpenGLScope_BeginEnd(GL_LINES);
        glVertex3fv(&Data.m_Start.x); glVertex3fv(&Data.m_End.x);
        //glVertex3fv(&Data.m_End.x);   glVertex3fv(&left.x);
        //glVertex3fv(&Data.m_End.x);   glVertex3fv(&right.x);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3fv(&Data.m_End.x); glVertex3fv(&LeftWing.x);
        glVertex3fv(&Data.m_End.x); glVertex3fv(&RightWing.x);
    }
    {
        GP_OpenGLScope_BeginEnd(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3fv(&Data.m_Start.x);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(&Data.m_End.x);
    }
}

static void gpDrawPolygonHelper(const gpDrawData::Polygon& Data, GLenum PolygonMode, const ezColor& Color)
{
    glPolygonMode(GL_FRONT_AND_BACK, PolygonMode);
    glColor4fv(&Color.r);

    auto uiNumVertices = Data.m_Vertices.GetCount();

    GP_OpenGLScope_BeginEnd(GL_POLYGON);

    for (decltype(uiNumVertices) i = 0; i < uiNumVertices; ++i)
    {
        glVertex3fv(&Data.m_Vertices[i].x);
    }
}

void gpDraw(const gpDrawData::Polygon& Data)
{
    EZ_ASSERT(Data.m_Vertices.GetCount() >= 3, "Need at least 3 vertices for a polygon!");

    if (!IsEqual(Data.m_FillColor.a, 0.0f))
    {
        gpDrawPolygonHelper(Data, GL_FILL, Data.m_FillColor);
    }

    if (!IsEqual(Data.m_OutlineColor.a, 0.0f))
    {
        glLineWidth(Data.m_fOutlineWidth);
        gpDrawPolygonHelper(Data, GL_LINE, Data.m_OutlineColor);
    }
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
    EZ_ASSERT(Data.m_uiNumLineSegments >= 3, "");

    if (!IsEqual(Data.m_FillColor.a, 0.0f))
    {
        gpDrawCircleHelper(Data, GL_FILL, Data.m_FillColor);
    }

    if (!IsEqual(Data.m_OutlineColor.a, 0.0f))
    {
        glLineWidth(Data.m_fOutlineWidth);
        gpDrawCircleHelper(Data, GL_LINE, Data.m_OutlineColor);
    }
}

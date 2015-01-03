#include "gpCore/PCH.h"
#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Utilities/EzMathExtensions.h"

using ezMath::Cos;
using ezMath::Sin;
using ezMath::IsZero;

void gpDraw(const gpDrawData::Point& Data)
{
    if (IsZero(Data.m_Color.a))
        return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glColor4fv(&Data.m_Color.r);
    glPointSize(Data.m_fPointSize);

    GP_OpenGLScope_BeginEnd(GL_POINTS);
    glVertex3fv(&Data.m_Position.x);
}

void gpDraw(const gpDrawData::Line& Data)
{
    if (IsZero(Data.m_Color.a))
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
    if (IsZero(Data.m_Color.a))
        return;

    gpVec3 BaseLine(1, 0, 0);
    auto Line = Data.m_End - Data.m_Start;
    Line.NormalizeIfNotZero();

    auto Angle = Line.GetAngleBetween(BaseLine);
    if(Line.y > 0.0f)
        Angle = -Angle;

    auto AngleLeftWing = Angle - Data.m_WingAngle;
    auto AngleRightWing = Angle + Data.m_WingAngle;

    auto LeftWing =  Data.m_End + gpVec3(-Cos(AngleLeftWing),  Sin(AngleLeftWing), 0.0f) * Data.m_fWingLength;
    auto RightWing = Data.m_End + gpVec3(-Cos(AngleRightWing), Sin(AngleRightWing), 0.0f) * Data.m_fWingLength;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor4fv(&Data.m_Color.r);

    glLineWidth(Data.m_fLineWidth);

    {
        GP_OpenGLScope_BeginEnd(GL_LINES);
        glVertex3fv(&Data.m_Start.x); glVertex3fv(&Data.m_End.x);
        glVertex3fv(&Data.m_End.x); glVertex3fv(&LeftWing.x);
        glVertex3fv(&Data.m_End.x); glVertex3fv(&RightWing.x);
    }
}

static void gpDrawPolygonHelper(const gpDrawData::Polygon& Data, GLenum PolygonMode, const ezColor& Color)
{
    glPolygonMode(GL_FRONT_AND_BACK, PolygonMode);
    glColor4fv(Color.GetData());

    auto uiNumVertices = Data.m_Vertices.GetCount();

    GP_OpenGLScope_BeginEnd(GL_POLYGON);

    for (decltype(uiNumVertices) i = 0; i < uiNumVertices; ++i)
    {
        glVertex3fv(Data.m_Vertices[i].GetData());
    }
}

void gpDraw(const gpDrawData::Polygon& Data)
{
    EZ_ASSERT(Data.m_Vertices.GetCount() >= 3, "Need at least 3 vertices for a polygon!");

    glMatrixMode(GL_MODELVIEW);

    float mModelView[16];
    gpAsArray(Data.m_Transform, mModelView, ezMatrixLayout::ColumnMajor);
    glLoadMatrixf(mModelView);

    if (!IsZero(Data.m_FillColor.a))
    {
        gpDrawPolygonHelper(Data, GL_FILL, Data.m_FillColor);
    }

    if (!IsZero(Data.m_OutlineColor.a))
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

    if (!IsZero(Data.m_FillColor.a))
    {
        gpDrawCircleHelper(Data, GL_FILL, Data.m_FillColor);
    }

    if (!IsZero(Data.m_OutlineColor.a))
    {
        glLineWidth(Data.m_fOutlineWidth);
        gpDrawCircleHelper(Data, GL_LINE, Data.m_OutlineColor);
    }
}

void gpDraw(const gpDrawData::Box& Data)
{
    gpVec3 Vertices[4];
    Vertices[0] = gpVec3(static_cast<gpScalar>(Data.m_Box.x),                    static_cast<gpScalar>(Data.m_Box.y),                     0.0f);
    Vertices[1] = gpVec3(static_cast<gpScalar>(Data.m_Box.x),                    static_cast<gpScalar>(Data.m_Box.y + Data.m_Box.height), 0.0f);
    Vertices[2] = gpVec3(static_cast<gpScalar>(Data.m_Box.x + Data.m_Box.width), static_cast<gpScalar>(Data.m_Box.y + Data.m_Box.height), 0.0f);
    Vertices[3] = gpVec3(static_cast<gpScalar>(Data.m_Box.x + Data.m_Box.width), static_cast<gpScalar>(Data.m_Box.y),                     0.0f);

    gpDrawData::Polygon Polygon;
    Polygon.m_Transform = gpIdentityTransform();
    Polygon.m_Vertices = Vertices;
    Polygon.m_FillColor = Data.m_FillColor;
    Polygon.m_OutlineColor = Data.m_OutlineColor;
    Polygon.m_fOutlineWidth = Data.m_fOutlineWidth;
    gpDraw(Polygon);
}

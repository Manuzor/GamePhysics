#include "gp/PCH.h"
#include "gp/Application.h"
#include "gp/Rendering/Rendering.h"

void gpApplication::RenderFrame()
{
    auto resolution = m_pWindow->GetResolution();
    glViewport(0, 0, resolution.width, resolution.height);

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(-20, 20, -20, 20, -10, 10);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glDisable(GL_CULL_FACE);

    glClearColor(0, 0, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GP_OpenGLScope_EnableDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    auto dt = ezClock::Get()->GetAccumulatedTime().GetSeconds();
    auto angle = ezAngle::Radian(static_cast<float>(-dt));
    angle.NormalizeRange();

    gpDrawData::Arrow line;
    line.m_Start.SetZero();
    line.m_End.Set(ezMath::Cos(angle), ezMath::Sin(angle), 0.0f);
    line.m_fLineWidth = angle.GetRadian() * 2;
    line.m_Color = ezColor(1.0f, 1.0f, 1.0f, 1.0f);
    gpDraw(line);

    gpDrawData::Arrow arrow;
    arrow.m_Start.Set(-0.25f, 0.25f, 0.0f);
    arrow.m_End.Set(-0.75f, 0.75f, 0.0f);
    arrow.m_fLineWidth = 1;
    arrow.m_Color = ezColor(0.0f, 1.0f, 1.0f, 1.0f);
    gpDraw(arrow);

    gpDrawData::Point point;
    point.m_Position.Set(0.5f, 0.5f, 0.0f);
    point.m_Color.SetRGB(ezVec3(1.0f, 0.0f, 0.0f));
    point.m_fPointSize = angle.GetRadian() * 2;
    gpDraw(point);

    gpDrawData::Circle circle;
    circle.m_Position.Set(-0.5f, -0.5f, 0.0f);
    circle.m_FillColor = ezColor(0.0f, 1.0f, 0.0f, 0.75f);
    circle.m_OutlineColor = ezColor(0.0f, 0.0f, 0.0f, 1.0f);
    circle.m_fOutlineWidth = angle.GetRadian() * 2;
    circle.m_fRadius = 0.5f;
    circle.m_uiNumLineSegments = 100;
    gpDraw(circle);

    gpDrawData::Polygon poly;
    poly.m_FillColor = ezColor(1.0f, 1.0f, 0.0f, 0.5f);
    poly.m_OutlineColor = ezColor(1.0f, 1.0f, 0.0f, 1.0f);
    static gpVec3 vertices[3] =
    {
        gpVec3( 0.00f,  0.00f, 0.00f),
        gpVec3(-0.25f,  0.25f, 0.00f),
        gpVec3(-0.25f, -0.25f, 0.00f),
    };
    poly.m_Vertices = vertices;
    gpDraw(poly);
}

#include "gp/PCH.h"
#include "gp/Application.h"

void gpApplication::RenderFrame()
{
    auto resolution = m_pWindow->GetResolution();
    glViewport(0, 0, resolution.width, resolution.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -10, 10);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_CULL_FACE);

    glClearColor(0, 0, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

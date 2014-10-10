#include "gp/PCH.h"
#include "gp/Application.h"

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

    glClearColor(0, 0, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(ezVec3) * m_VertexBufferData.GetCount(),
                    &m_VertexBufferData[0]);
    glVertexAttribPointer(
        0,                             // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,
        GL_FLOAT,                      // Type
        GL_FALSE,                      // Normalized?
        0,                             // Stride
        nullptr                        // Array buffer offset.
    );

    glDrawArrays(GL_TRIANGLES, 0, m_VertexBufferData.GetCount());

    glDisableVertexAttribArray(0);
}

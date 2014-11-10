#include "gpCore/PCH.h"

#include <Foundation/Configuration/CVar.h>

#include "gpCore/Window.h"
#include "gpCore/Rendering/Renderer.h"

ezCVarInt gpWindow::s_iWidth("Window.Width", 500, ezCVarFlags::Default, "Width of the window.");
ezCVarInt gpWindow::s_iHeight("Window.Height", 500, ezCVarFlags::Default, "Height of the window.");

gpWindow::gpWindow(const char* szTitle)
{
    m_CreationDescription.m_WindowPosition.Set(400, 100);
    m_CreationDescription.m_ClientAreaSize.width = 500;
    m_CreationDescription.m_ClientAreaSize.height = 500;
    m_CreationDescription.m_Title = szTitle;
    m_CreationDescription.m_bFullscreenWindow = false;
    m_CreationDescription.m_bResizable = true;

    gpRenderer::SetResolution(m_CreationDescription.m_ClientAreaSize);
}

gpWindow::~gpWindow()
{
    Destroy();
}

ezResult gpWindow::Initialize()
{
    ezWindow::Initialize();
    return CreateGraphicsContext();
}

ezSizeU32 gpWindow::GetResolution() const
{
    return m_CreationDescription.m_ClientAreaSize;
}

void gpWindow::OnResizeMessage(const ezSizeU32& newWindowSize)
{
    printf("Resolution changed: %dx%d\n", newWindowSize.width, newWindowSize.height);

    m_CreationDescription.m_ClientAreaSize = newWindowSize;

    gpRenderer::SetResolution(newWindowSize);
}

void gpWindow::OnClickCloseMessage()
{
    EventData data;
    data.m_Reason = ClickClose;
    m_WindowEvent.Broadcast(&data);
}

void gpWindow::OnFocusMessage(bool bHasFocus)
{
    EventData data;
    data.m_Reason = bHasFocus ? FocusGained : FocusLost;
    m_WindowEvent.Broadcast(&data);
}

void gpWindow::PresentFrame()
{
    ::SwapBuffers(m_hDeviceContext);
}

#if EZ_ENABLED(GP_AsyncRendering)
#include "gpCore/Implementation/Window_AsyncGraphicsContext.inl"
#else
#include "gpCore/Implementation/Window_GraphicsContext.inl"
#endif

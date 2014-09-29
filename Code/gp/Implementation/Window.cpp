#include "gp/PCH.h"
#include "gp/Window.h"

gpWindow::gpWindow()
{
    m_CreationDescription.m_WindowPosition.Set(400, 200);
    m_CreationDescription.m_ClientAreaSize.width = 500;
    m_CreationDescription.m_ClientAreaSize.height = 500;
    m_CreationDescription.m_Title = "Game Physics mm198";
    m_CreationDescription.m_bFullscreenWindow = false;
    m_CreationDescription.m_bResizable = true;
    Initialize();

    auto hWindow = GetNativeWindowHandle();
#if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
    SetWindowPos(hWindow,
                 nullptr,
                 m_CreationDescription.m_WindowPosition.x,
                 m_CreationDescription.m_WindowPosition.y,
                 0,
                 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION);
#else
#error "Unsupported for this platform."
#endif
}

gpWindow::~gpWindow()
{
    Destroy();
}

ezSizeU32 gpWindow::GetResolution() const
{
    return m_CreationDescription.m_ClientAreaSize;
}

void gpWindow::OnResizeMessage(const ezSizeU32& newWindowSize)
{
    printf("Resolution changed: %dx%d\n", newWindowSize.width, newWindowSize.height);

    m_CreationDescription.m_ClientAreaSize = newWindowSize;
}

void gpWindow::OnClickCloseMessage()
{
    EventData data;
    data.m_Reason = ClickClose;
    m_WindowEvent.Broadcast(&data);
}

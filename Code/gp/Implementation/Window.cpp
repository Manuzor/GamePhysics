#include "gp/PCH.h"

#include <Foundation/Configuration/CVar.h>

#include "gp/Window.h"
#include "gp/Rendering/Renderer.h"

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

void gpWindow::PresentFrame()
{
    ::SwapBuffers(m_hDeviceContext);
}

ezResult gpWindow::CreateGraphicsContext()
{
    EZ_LOG_BLOCK("CreateGraphicsContext", "OpenGL");
    return CreateContextOpenGL();
}

ezResult gpWindow::DestroyGraphicsContext()
{
    return DestroyContextOpenGL();
}

ezResult gpWindow::CreateContextOpenGL()
{
    int iColorBits = 24;
    int iDepthBits = 24;
    int iBPC = 8;

    HWND hWindow = GetNativeWindowHandle();

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof (PIXELFORMATDESCRIPTOR),
        1, // Version
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE, // Flags
        PFD_TYPE_RGBA, // Pixeltype
        iColorBits, // Color Bits
        iBPC, 0, iBPC, 0, iBPC, 0, iBPC, 0,// Red Bits / Red Shift, Green Bits / Shift, Blue Bits / Shift, Alpha Bits / Shift
        0, 0, 0, 0, 0, // Accum Bits (total), Accum Bits Red, Green, Blue, Alpha
        iDepthBits, 8, // Depth, Stencil Bits
        0, // Aux Buffers
        PFD_MAIN_PLANE, // Layer Type (ignored)
        0, 0, 0, 0 // ignored deprecated flags
    };

    m_hDeviceContext = GetDC(hWindow);

    if (m_hDeviceContext == nullptr)
    {
        ezLog::Error("Could not retrieve the Window DC");
        goto failure;
    }

    int iPixelformat = ChoosePixelFormat(m_hDeviceContext, &pfd);
    if (iPixelformat == 0)
    {
        ezLog::Error("ChoosePixelFormat failed.");
        goto failure;
    }

    if (!SetPixelFormat(m_hDeviceContext, iPixelformat, &pfd))
    {
        ezLog::Error("SetPixelFormat failed.");
        goto failure;
    }

    m_hRenderContext = wglCreateContext(m_hDeviceContext);
    if (m_hRenderContext == nullptr)
    {
        ezLog::Error("wglCreateContext failed.");
        goto failure;
    }

    if (!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
    {
        ezLog::Error("wglMakeCurrent failed.");
        goto failure;
    }

    SetFocus(hWindow);
    SetForegroundWindow(hWindow);

    ezLog::Success("OpenGL graphics context is initialized.");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        ezLog::Error("Failed to initialize GLEW.");
        goto failure;
    }

    ezLog::Success("GLEW is initialized.");

    auto szRenderer = glGetString(GL_RENDERER);
    auto szVersion = glGetString(GL_VERSION);

    ezLog::Info("Render Device:  %s", szRenderer);
    ezLog::Info("OpenGL Version: %s", szVersion);

    return EZ_SUCCESS;

failure:
    ezLog::Error("Failed to initialize the graphics context.");

    DestroyContextOpenGL();
    return EZ_FAILURE;
}

ezResult gpWindow::DestroyContextOpenGL()
{
    EZ_LOG_BLOCK("gpWindow::DestroyContextOpenGL");

    if (!m_hRenderContext && !m_hDeviceContext)
        return EZ_SUCCESS;

    if (m_hRenderContext)
    {
        ezLog::Dev("Destroying the Render Context.");

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRenderContext);
        m_hRenderContext = NULL;
    }

    if (m_hDeviceContext)
    {
        ezLog::Dev("Destroying the Device Context.");

        ReleaseDC(GetNativeWindowHandle(), m_hDeviceContext);
        m_hDeviceContext = NULL;
    }

    ezLog::Success("OpenGL graphics context is destroyed.");

    return EZ_SUCCESS;
}

#include "gp/PCH.h"
#include "gp/Application.h"
#include "gp/Window.h"

#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Time/Clock.h>

gpApplication::gpApplication() :
    m_pMainAllocator(nullptr),
    m_pWindow(nullptr),
    m_bQuit(false)
{
}

gpApplication::~gpApplication()
{
    m_pWindow = nullptr;
    m_pMainAllocator = nullptr;
}

void gpApplication::AfterEngineInit()
{
    // Setup the logging system
    ezGlobalLog::AddLogWriter(ezLogWriter::Console::LogMessageHandler);
    ezGlobalLog::AddLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);

    m_pMainAllocator = ezFoundation::GetDefaultAllocator();

    m_pWindow = EZ_DEFAULT_NEW(gpWindow);
    m_pWindow->AddEventHandler(gpWindow::Event::Handler(&gpApplication::WindowEventHandler, this));

    ezStartup::StartupEngine();

    ezClock::SetNumGlobalClocks();
    ezClock::Get()->SetTimeStepSmoothing(&m_TimeStepSmoother);
}

void gpApplication::BeforeEngineShutdown()
{
    ezStartup::ShutdownEngine();

    EZ_DEFAULT_DELETE(m_pWindow);
    m_pMainAllocator = nullptr;
}

ezApplication::ApplicationExecution gpApplication::Run()
{
    m_pWindow->ProcessWindowMessages();

    if (m_bQuit)
        return Quit;

    ezClock::UpdateAllGlobalClocks();
    return Continue;
}

void gpApplication::WindowEventHandler(gpWindow::EventData* pEventData)
{
    switch(pEventData->m_Reason)
    {
    case gpWindow::ClickClose:
        m_bQuit = true;
        break;
    default:
        break;
    }
}

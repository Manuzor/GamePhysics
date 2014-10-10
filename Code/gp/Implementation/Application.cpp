#include "gp/PCH.h"
#include "gp/Application.h"
#include "gp/Window.h"

#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Time/Clock.h>
#include <Core/Input/InputManager.h>
#include <Foundation/System/SystemInformation.h>

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

    auto& SysInfo = ezSystemInformation::Get();
    {
        EZ_LOG_BLOCK("System Information");
        ezLog::Info("Configuration:    %s", SysInfo.GetBuildConfiguration());
        ezLog::Info("Host Name:        %s", SysInfo.GetHostName());
        ezLog::Info("Platform:         %s %s", SysInfo.GetPlatformName(), SysInfo.Is64BitOS() ? "64 bit" : "32 bit");
        ezLog::Info("CPU Cores:        %u", SysInfo.GetCPUCoreCount());
        ezLog::Info("Main Memory:      %f GiB (%u Bytes)",
                    SysInfo.GetInstalledMainMemory() / float(1024 * 1024 * 1024),
                    SysInfo.GetInstalledMainMemory());
        ezLog::Info("Memory Page Size: %u", SysInfo.GetMemoryPageSize());
    }

    m_pMainAllocator = ezFoundation::GetDefaultAllocator();

    m_pWindow = EZ_DEFAULT_NEW(gpWindow);
    m_pWindow->AddEventHandler(gpWindow::Event::Handler(&gpApplication::WindowEventHandler, this));

    ezStartup::StartupEngine();

    ezClock::SetNumGlobalClocks();
    ezClock::Get()->SetTimeStepSmoothing(&m_TimeStepSmoother);

    SetupInput();

    m_LastUpdate = ezTime::Now();

    ezLog::Success("Initialization complete.");
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

    const auto tUpdateInterval = ezTime::Seconds(1.0 / 60.0);
    const auto tNow = ezTime::Now();

    bool bInputUpdated = false;

    while(tNow - m_LastUpdate > tUpdateInterval)
    {
        bInputUpdated = true;

        UpdateInput(tUpdateInterval);

        m_LastUpdate += tUpdateInterval;
    }

    if (!bInputUpdated)
    {
        ezInputManager::PollHardware();
    }

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

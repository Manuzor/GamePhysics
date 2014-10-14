#include "gp/PCH.h"
#include "gp/Application.h"
#include "gp/Window.h"
#include "gp/Task.h"

#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Time/Clock.h>
#include <Core/Input/InputManager.h>
#include <Foundation/System/SystemInformation.h>
#include <Foundation/Threading/TaskSystem.h>

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

    EZ_LOG_BLOCK("Initialization");

    auto logLevel = ezLogMsgType::All;
    ezGlobalLog::SetLogLevel(ezLogMsgType::All);
    ezLog::Info("Log Level: %u", logLevel);

    m_pMainAllocator = ezFoundation::GetDefaultAllocator();

    m_pWindow = EZ_DEFAULT_NEW(gpWindow);
    m_pWindow->AddEventHandler(gpWindow::Event::Handler(&gpApplication::WindowEventHandler, this));

    ezStartup::StartupEngine();

    ezClock::SetNumGlobalClocks();
    ezClock::Get()->SetTimeStepSmoothing(&m_TimeStepSmoother);

    SetupInput();

    m_LastUpdate = ezTime::Now();

    glGenVertexArrays(1, &m_uiVertexArrayID);
    glBindVertexArray(m_uiVertexArrayID);

    m_VertexBufferData.SetCountUninitialized(3);
    m_VertexBufferData[0].Set(-1.0f, -1.0f, 0.0f);
    m_VertexBufferData[1].Set( 1.0f, -1.0f, 0.0f);
    m_VertexBufferData[2].Set( 0.0f,  1.0f, 0.0f);

    glGenBuffers(1, &m_uiVertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBufferID);
    EZ_CHECK_AT_COMPILETIME(sizeof(ezVec3) == 3 * sizeof(float));
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ezVec3) * m_VertexBufferData.GetCount(),
                 &m_VertexBufferData[0],
                 GL_DYNAMIC_DRAW);

    static gpSimpleTask Task0([]{ ezThreadUtils::Sleep(200); ezLog::Success("Finished!"); });
    static gpSimpleTask Task1([]{ ezThreadUtils::Sleep(200); ezLog::Success("First task done!"); });
    static gpSimpleTask Task2([]{ ezThreadUtils::Sleep(200); ezLog::Success("Second task done!"); });

    auto Group = ezTaskSystem::CreateTaskGroup(ezTaskPriority::LongRunning);
    ezTaskSystem::AddTaskToGroup(Group, &Task1);
    ezTaskSystem::AddTaskToGroup(Group, &Task2);

    ezTaskSystem::StartSingleTask(&Task0, ezTaskPriority::LongRunning, Group);
    ezTaskSystem::StartTaskGroup(Group);
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

    auto& y = m_VertexBufferData[2].y;
    if (y > 0.0f)
    {
        ezLog::Dev("Y: %f", y);
        y -= 0.01f;
    }

    RenderFrame();

    m_pWindow->PresentFrame();

    ezTaskSystem::FinishFrameTasks();

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

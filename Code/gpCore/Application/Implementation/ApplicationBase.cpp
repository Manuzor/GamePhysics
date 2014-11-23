#include "gpCore/PCH.h"

#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Time/Clock.h>
#include <Core/Input/InputManager.h>
#include <Foundation/System/SystemInformation.h>
#include <Foundation/Threading/TaskSystem.h>
#include <Foundation/IO/FileSystem/DataDirTypeFolder.h>

#include "gpCore/Application/ApplicationBase.h"
#include "gpCore/Window.h"
#include "gpCore/Task.h"
#include "gpCore/Rendering/Renderer.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/Utilities/FileSystem.h"

gpApplicationBase::gpApplicationBase(const char* szTitle) :
    m_szTitle(szTitle),
    m_pWindow(nullptr),
    m_bQuit(false),
    m_bRegisteredLogging(false)
{
}

gpApplicationBase::~gpApplicationBase()
{
    m_pWindow = nullptr;
    m_szTitle = nullptr;
}

void gpApplicationBase::SetupFileSystem()
{
    ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

    // Log files dir
    {
        ezStringBuilder sDir = gpGetApplicationDirectory();
        sDir.AppendPath("Logs");
        ezOSFile::CreateDirectoryStructure(sDir);
        auto result = ezFileSystem::AddDataDirectory(sDir, ezFileSystem::AllowWrites, "Logs", "Logs");
        EZ_VERIFY(result.Succeeded(), "Failed to mount Logs directory");
    }

    // Settings dir
    {
        ezStringBuilder sDir = gpGetWorkingDirectory();
        sDir.AppendPath("Data", "Settings");
        auto result = ezFileSystem::AddDataDirectory(sDir, ezFileSystem::AllowWrites, "Settings", "Settings");
        EZ_VERIFY(result.Succeeded(), "Failed to mount Settings directory");
    }
}

void gpApplicationBase::SetupLogging()
{
    EZ_LOG_BLOCK("Setup");

    // Assemble log file path for the text file log writer
    {
        ezStringBuilder sbAbsFileName = GetArgument(0);
        ezStringBuilder sbLogFileName = "<Logs>";
        sbLogFileName.Append(sbAbsFileName.GetFileNameAndExtension().GetData(), ".log");
        m_TextFileLogger.BeginLog(sbLogFileName);
    }

    // Setup the logging system
    ezGlobalLog::AddLogWriter(ezLogWriter::Console::LogMessageHandler);
    ezGlobalLog::AddLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);
    ezGlobalLog::AddLogWriter({ &ezLogWriter::TextFile::LogMessageHandler, &m_TextFileLogger });

    auto logLevel = ezLogMsgType::All;
    ezGlobalLog::SetLogLevel(logLevel);
    ezLog::Info("Log Level: %u", logLevel);

    m_bRegisteredLogging = true;
}

void gpApplicationBase::SetupWindow()
{
    m_pWindow = EZ_DEFAULT_NEW(gpWindow)(m_szTitle);
    auto result = m_pWindow->Initialize();
    EZ_ASSERT(result.Succeeded(), "Window initialization failed.");
    m_pWindow->AddEventHandler(gpWindow::Event::Handler(AddressOf(gpApplicationBase::WindowEventHandler), this));
}

void gpApplicationBase::LogSystemInformation()
{
    EZ_LOG_BLOCK("System Information");
    auto& SysInfo = ezSystemInformation::Get();
    ezLog::Info("Configuration:    %s", SysInfo.GetBuildConfiguration());
    ezLog::Info("Host Name:        %s", SysInfo.GetHostName());
    ezLog::Info("Platform:         %s %s", SysInfo.GetPlatformName(), SysInfo.Is64BitOS() ? "64 bit" : "32 bit");
    ezLog::Info("CPU Cores:        %u", SysInfo.GetCPUCoreCount());
    ezLog::Info("Main Memory:      %f GiB (%u Bytes)",
                SysInfo.GetInstalledMainMemory() / float(1024 * 1024 * 1024),
                SysInfo.GetInstalledMainMemory());
    ezLog::Info("Memory Page Size: %u", SysInfo.GetMemoryPageSize());
}

void gpApplicationBase::Cleanup()
{
    EZ_DEFAULT_DELETE(m_pWindow);

    if(m_bRegisteredLogging)
    {
        ezLog::Success("Good bye!");
        m_TextFileLogger.EndLog();
        ezGlobalLog::RemoveLogWriter(ezLoggingEvent::Handler(&ezLogWriter::TextFile::LogMessageHandler, &m_TextFileLogger));
        ezGlobalLog::RemoveLogWriter(ezLogWriter::Console::LogMessageHandler);
        ezGlobalLog::RemoveLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);
    }

    ezFileSystem::ClearAllDataDirectories();
    ezFileSystem::ClearAllDataDirectoryFactories();
}

void gpApplicationBase::WindowEventHandler(gpWindow::EventData* pEventData)
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

void gpApplicationBase::SetupRendering()
{
}

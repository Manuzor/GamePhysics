#include "gp/PCH.h"

#include <Core/Input/InputManager.h>
#include <Foundation/Communication/Telemetry.h>

#include "gp/Application/AndyPatricles.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/RenderExtractor.h"
#include "gp/World/World.h"
#include "gp/World/Particle.h"

void gpAndyParticlesApp::AfterEngineInit()
{
    SetupLogging();
    LogSystemInformation();

    {
        EZ_LOG_BLOCK("Initialization");

        ezTelemetry::CreateServer();

        if (ezPlugin::LoadPlugin("ezInspectorPlugin").Failed())
        {
            ezLog::SeriousWarning("Failed to load ezInspectorPlugin.");
        }

        SetupWindow();
        ezStartup::StartupEngine();
        ezClock::SetNumGlobalClocks();
        ezClock::Get()->SetTimeStepSmoothing(AddressOf(m_TimeStepSmoother));
        m_LastUpdate = ezTime::Now();
        SetupInput();
        // Poll once to finish input initialization;
        ezInputManager::PollHardware();
        SetupRendering();
    }

    RunTestsIfEnabled();

    m_pWorld = EZ_DEFAULT_NEW(gpWorld)("PrimaryWorld");
    m_pWorld->SetGravity(gpVec3(0, -9.81f, 0));
    gpRenderExtractor::AddExtractionListener(
        gpRenderExtractionListener(&gpWorld::ExtractRenderingData, m_pWorld));
    PopulateWorld();
}

void gpAndyParticlesApp::BeforeEngineShutdown()
{
    EZ_DEFAULT_DELETE(m_pWorld);

    ezStartup::ShutdownEngine();
    Cleanup();

    ezPlugin::UnloadPlugin("ezInspectorPlugin");

    ezTelemetry::CloseConnection();
}

ezApplication::ApplicationExecution gpAndyParticlesApp::Run()
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
        m_pWorld->StepSimulation(tUpdateInterval);

        m_LastUpdate += tUpdateInterval;
    }

    // Make sure we don't miss any input events.
    if (!bInputUpdated)
    {
        ezInputManager::PollHardware();
    }

    RenderFrame();

    m_pWindow->PresentFrame();

    ezTaskSystem::FinishFrameTasks();

    ezTelemetry::PerFrameUpdate();

    return Continue;
}

void gpAndyParticlesApp::PopulateWorld()
{
    auto pParticle = m_pWorld->CreateEntity<gpParticleEntity>();
    pParticle->SetName("TheParticle");
    pParticle->SetLinearVelocity(gpVec3(10, 10, 0));
    auto result = m_pWorld->AddEntity(pParticle);
    EZ_ASSERT(result.Succeeded(), "");
    m_pWorld->GetEntityDrawInfo(pParticle).m_Color = ezColor(1, 0, 0, 0.9f);
}

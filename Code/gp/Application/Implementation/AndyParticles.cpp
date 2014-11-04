#include "gp/PCH.h"

#include <Core/Input/InputManager.h>
#include <Foundation/Communication/Telemetry.h>
#include <Foundation/Utilities/Stats.h>

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
        RegisterInputAction("Game", "Click", ezInputSlot_MouseButton0);
        SetupRendering();
    }

    RunTestsIfEnabled();

    m_pWorld = EZ_DEFAULT_NEW(gpWorld)("PrimaryWorld");
    //m_pWorld->SetGravity(gpVec3(0, -9.81f, 0));
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
        Update(tUpdateInterval);
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
    pParticle->SetPosition(gpVec3(100, 200, 0));
    //pParticle->SetLinearVelocity(gpVec3(10, 10, 0));
    auto result = m_pWorld->AddEntity(pParticle);
    EZ_ASSERT(result.Succeeded(), "");
    m_pWorld->GetEntityDrawInfo(pParticle).m_Color = ezColor(1, 0, 0, 0.9f);
}

void gpAndyParticlesApp::Update(ezTime dt)
{
    static bool bAddedParticle = false;

    auto GameClickState = ezInputManager::GetInputActionState("Game", "Click");
    if (GameClickState != ezKeyState::Pressed)
        return;

    if(!bAddedParticle)
    {
        float fX;
        ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &fX);
        fX *= gpWindow::GetWidthCVar()->GetValue();

        float fY;
        ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &fY);
        fY *= gpWindow::GetHeightCVar()->GetValue();

        gpRenderExtractor::AddExtractionListener(gpRenderExtractionListener(&gpAndyParticlesApp::ExtractVelocityData, this));

        AddNewParticle(gpVec3(fX, fY, 0.0f));
        bAddedParticle = true;

        // Stat
        ezStringBuilder sbStat;
        sbStat.Format("{%.2f, %.2f}", fX, fY);
        ezStats::SetStat("Click/Start", sbStat.GetData());
    }
    else
    {
        EZ_ASSERT(m_pCurrentParticle, "Did we miss Pressed input?");

        float fX;
        ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &fX);
        fX *= gpWindow::GetWidthCVar()->GetValue();

        float fY;
        ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &fY);
        fY *= gpWindow::GetHeightCVar()->GetValue();

        gpVec3 MousePos(fX, fY, 0.0f);
        auto Velocity = MousePos - m_pCurrentParticle->GetPosition();
        m_pCurrentParticle->SetLinearVelocity(Velocity);
        bAddedParticle = false;

        gpRenderExtractor::RemoveExtractionListener(gpRenderExtractionListener(&gpAndyParticlesApp::ExtractVelocityData, this));

        // Stat
        ezStringBuilder sbStat;
        sbStat.Format("{%.2f, %.2f}", fX, fY);
        ezStats::SetStat("Click/End", sbStat.GetData());
    }
}

void gpAndyParticlesApp::ExtractVelocityData(gpRenderExtractor* pExtractor)
{
    auto pLine = pExtractor->AllocateRenderData<gpDrawData::Line>();
    pLine->m_Start = m_pCurrentParticle->GetPosition();
    pLine->m_End.SetZero();
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &pLine->m_End.x);
    pLine->m_End.x *= gpWindow::GetWidthCVar()->GetValue();

    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &pLine->m_End.y);
    pLine->m_End.y *= gpWindow::GetHeightCVar()->GetValue();

}

void gpAndyParticlesApp::AddNewParticle(gpVec3 Position)
{
    static ezUInt32 uiCount = 0;
    m_pCurrentParticle = m_pWorld->CreateEntity<gpParticleEntity>();
    {
        ezStringBuilder s;
        s.AppendFormat("Particle #%u", uiCount++);
        m_pCurrentParticle->SetName(s.GetData());
    }
    m_pCurrentParticle->AddRef();
    m_pCurrentParticle->SetPosition(Position);
    EZ_VERIFY(m_pWorld->AddEntity(m_pCurrentParticle).Succeeded(), "Failed to add new particle?!");
    m_pWorld->GetEntityDrawInfo(m_pCurrentParticle).m_Color = ezColor(1, 0, 0, 0.9f);
    ezLog::Success("Added new particle %s @ {%.3f, %.3f, %.3f}",
                   m_pCurrentParticle->GetName().GetData(),
                   Position.x, Position.y, Position.z);
}

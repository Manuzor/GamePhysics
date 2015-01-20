#include "gpAndyParticles/PCH.h"

#include <Core/Input/InputManager.h>
#include <Foundation/Communication/Telemetry.h>
#include <Foundation/Utilities/Stats.h>

#include "gpAndyParticles/Application.h"
#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/World/World.h"

void gpAndyParticlesApp::AfterEngineInit()
{
    SetupFileSystem();
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
        RegisterInputAction("Game", "Spawn", ezInputSlot_MouseButton0);
        RegisterInputAction("Game", "CancelSpawn", ezInputSlot_MouseButton1);
        SetupRendering();
    }

    m_pWorld = EZ_DEFAULT_NEW(gpWorld)("PrimaryWorld");
    EZ_ASSERT(m_pWorld, "Failed to create world.");
    gpGravityOf(Deref(m_pWorld)) = gpLinearAcceleration(gpVec3(0, 9.81f, 0));
    gpRenderExtractor::AddExtractionListener([=](gpRenderExtractor* pExtractor){
        gpExtractRenderDataOf(Deref(m_pWorld), pExtractor);
    });
    PopulateWorld();

    {
        EZ_LOG_BLOCK("Controls", "Keyboard & Mouse");
        ezLog::Info("Escape Key  -> Exit Game");
        ezLog::Info("Left Mouse  -> Spawn or Fire Player");
        ezLog::Info("Right Mouse -> Despawn Player");
    }
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
        gpStepSimulationOf(Deref(m_pWorld), tUpdateInterval);

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
    auto pParticle = gpNew<gpEntity>(Deref(m_pWorld));
    EZ_ASSERT(pParticle, "Failed to create particle");
    auto& particle = Deref(pParticle);
    gpNameOf(particle) = "TheParticle";
    gpSet(gpPositionOf(particle), 100, 200, 0);
    //gpLinearVelocityOf(particle).Set(10, 10, 0);
    auto result = gpAddTo(Deref(m_pWorld), particle);
    EZ_ASSERT(result.Succeeded(), "");
    auto& DrawInfo = gpDrawInfoOf(Deref(m_pWorld), particle);
    DrawInfo.m_Color = ezColor(1, 0, 0, 0.9f);
    DrawInfo.m_fScale = 8.0f;
}

void gpAndyParticlesApp::Update(ezTime dt)
{
    static bool bAddedParticle = false;

    auto ResetSpawning = [&]{
        bAddedParticle = false;
        gpRenderExtractor::RemoveExtractionListener(gpRenderExtractionListener(&gpAndyParticlesApp::ExtractVelocityData, this));
    };

    if (ezInputManager::GetInputActionState("Game", "CancelSpawn") == ezKeyState::Pressed
        && bAddedParticle)
    {
        ResetSpawning();
        auto& currentParticle = Deref(m_pCurrentParticle);
        EZ_VERIFY(gpRemoveFrom(Deref(m_pWorld), currentParticle).Succeeded(), "Failed to remove the current particle!");
        const auto& pos = gpValueOf(gpPositionOf(currentParticle));
        ezLog::Info("Removed particle   '%s' @ {%.3f, %.3f, %.3f}",
                    gpNameOf(currentParticle).GetData(),
                    pos.x, pos.y, pos.z);
        gpReleaseReferenceTo(currentParticle);
        m_pCurrentParticle = nullptr;
        gpCollectGarbageOf(Deref(m_pWorld));
    }

    if (ezInputManager::GetInputActionState("Game", "Spawn") != ezKeyState::Pressed)
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

        AddNewParticle(gpDisplacement(fX, fY, 0.0f));
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

        // Set the new linear velocity of the particle.
        gpVec3 MousePos(fX, fY, 0.0f);
        auto& currentParticle = Deref(m_pCurrentParticle);
        gpLinearVelocityOf(currentParticle) = gpLinearVelocity(MousePos - gpValueOf(gpPositionOf(currentParticle)));
        gpGravityFactorOf(currentParticle) = 1.0f;

        ResetSpawning();

        // Stat
        ezStringBuilder sbStat;
        sbStat.Format("{%.2f, %.2f}", fX, fY);
        ezStats::SetStat("Click/End", sbStat.GetData());
    }
}

void gpAndyParticlesApp::ExtractVelocityData(gpRenderExtractor* pExtractor)
{
    auto pLine = pExtractor->AllocateRenderData<gpDrawData::Line>();
    pLine->m_Start = gpValueOf(gpPositionOf(Deref(m_pCurrentParticle)));
    pLine->m_End.SetZero();
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &pLine->m_End.x);
    pLine->m_End.x *= gpWindow::GetWidthCVar()->GetValue();

    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &pLine->m_End.y);
    pLine->m_End.y *= gpWindow::GetHeightCVar()->GetValue();

}

void gpAndyParticlesApp::AddNewParticle(const gpDisplacement& Position)
{
    static ezUInt32 uiCount = 0;
    m_pCurrentParticle = gpNew<gpEntity>(Deref(m_pWorld));
    EZ_ASSERT(m_pCurrentParticle, "Failed to create new particle");

    auto& particle = Deref(m_pCurrentParticle);
    {
        ezStringBuilder s;
        s.AppendFormat("Particle #%u", uiCount++);
        gpNameOf(particle) = s;
    }
    gpPositionOf(particle) = Position;
    gpGravityFactorOf(particle) = 0.0f;
    EZ_VERIFY(gpAddTo(Deref(m_pWorld), particle).Succeeded(), "Failed to add new particle?!");
    ezLog::Success("Added new particle '%s' @ {%.3f, %.3f, %.3f}",
                   gpNameOf(particle).GetData(),
                   gpX(Position), gpY(Position), gpZ(Position));
}

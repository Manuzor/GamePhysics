#include "gpAndyForceFields/PCH.h"

#include <Core/Input/InputManager.h>
#include <Foundation/Communication/Telemetry.h>
#include <Foundation/Utilities/Stats.h>
#include <Foundation/Threading/TaskSystem.h>
#include <Foundation/Time/Stopwatch.h>

#include "gpAndyForceFields/ForceFieldsApp.h"
#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/World/World.h"
#include "gpCore/World/Particle.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/Shapes/Rectangle.h"
#include "gpCore/Task.h"
#include "gpCore/World/RigidBody.h"

ezCVarFloat gpAndyForceFieldsApp::s_fPlayerMaxSpeed("PlayerMaxSpeed", 150.0f, ezCVarFlags::Default, "The maximum speed the player may reach via user input.");

ezCVarInt g_iPlayerSpawnAreaWidth("PlayerSpawnAreaWidth", 150, ezCVarFlags::Default, "Width of the spawn area of the player.");
ezCVarInt g_iPlayerSpawnAreaHeight("PlayerSpawnAreaHeight", 150, ezCVarFlags::Default, "Width of the spawn area of the player.");

ezColor g_SpawnAreaColor(1.0f, 0.0f, 0.0f, 0.1f);

ezStopwatch g_StopWatch;

gpRigidBody* g_pPlayerTarget = nullptr;
ezDynamicArray<gpForceFieldEntity*> g_ForceFields;

static void SpawnTarget(gpWorld* pWorld)
{
    EZ_ASSERT(g_pPlayerTarget, "Target not spawned yet.");
    EZ_ASSERT(g_pPlayerTarget->GetWorld() == nullptr, "Target is already spawned.");

    // \todo Random position for target.
    g_pPlayerTarget->GetProperties()->m_Position.Set(450, 50, 0);

    pWorld->AddEntity(g_pPlayerTarget);
    pWorld->GetEntityDrawInfo(g_pPlayerTarget).m_Color = ezColor::GetYellow();

    ezLog::Success("Target spawned.");
}

static void DespawnTarget(gpWorld* pWorld)
{
    if (g_pPlayerTarget->GetWorld() == pWorld)
    {
        pWorld->RemoveEntity(g_pPlayerTarget);
        ezLog::Info("Target despawned.");
    }
}

static void CreateTarget(gpWorld* pWorld)
{
    static gpCircleShape* pCircle;

    if(pCircle == nullptr)
    {
        static gpCircleShape Circle;
        Circle.SetRadius(40.0f);
        pCircle = &Circle;
    }

    g_pPlayerTarget = pWorld->CreateEntity<gpRigidBody>();
    g_pPlayerTarget->AddRef();
    g_pPlayerTarget->SetName("Target");
    g_pPlayerTarget->SetShape(pCircle);
    auto pProps = g_pPlayerTarget->GetProperties();
    pProps->m_fGravityFactor = 0.0f;

    ezLog::Success("Created target.");

    SpawnTarget(pWorld);
}

static void ExtractSpawnData(gpRenderExtractor* pExtractor)
{
    auto pArea = pExtractor->AllocateRenderData<gpDrawData::Box>();
    pArea->m_Box.x = 0;
    pArea->m_Box.y = gpWindow::GetHeightCVar()->GetValue();
    pArea->m_Box.width = g_iPlayerSpawnAreaWidth.GetValue();
    pArea->m_Box.height = -g_iPlayerSpawnAreaHeight.GetValue();
    pArea->m_OutlineColor = g_SpawnAreaColor; pArea->m_OutlineColor.a *= 2.0f;
    pArea->m_FillColor = g_SpawnAreaColor;
}

static gpVec3 GetMousePosition()
{
    float fX;
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &fX);
    fX *= gpWindow::GetWidthCVar()->GetValue();

    float fY;
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &fY);
    fY *= gpWindow::GetHeightCVar()->GetValue();

    return gpVec3(fX, fY, 0.0f);
}

static bool g_bSkipUpdate = false;

void gpAndyForceFieldsApp::AfterEngineInit()
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
        RegisterInputAction("Game", "Spawn", ezInputSlot_MouseButton0);
        RegisterInputAction("Game", "CancelSpawn", ezInputSlot_MouseButton1);
        // Poll once to finish input initialization
        ezInputManager::PollHardware();
        SetupRendering();
    }

    m_pWorld = EZ_DEFAULT_NEW(gpWorld)("PrimaryWorld");
    m_pWorld->SetGravity(gpVec3(0, 9.81f, 0));
    gpRenderExtractor::AddExtractionListener(
        gpRenderExtractionListener(&gpWorld::ExtractRenderingData, m_pWorld));
    gpRenderExtractor::AddExtractionListener(ExtractSpawnData);
    CreatePlayer();
    CreateForceFields();
    CreateTarget(m_pWorld);

    m_pWindow->AddEventHandler([](gpWindow::EventData* pEvent){ if(pEvent->m_Reason == gpWindow::FocusGained) g_bSkipUpdate = true; });

    {
        EZ_LOG_BLOCK("Controls", "Keyboard & Mouse");
        ezLog::Info("Escape Key  -> Exit Game");
        ezLog::Info("Left Mouse  -> Spawn or Fire Player");
        ezLog::Info("Right Mouse -> Despawn Player");
    }

    g_StopWatch.StopAndReset();
}

void gpAndyForceFieldsApp::BeforeEngineShutdown()
{
    g_pPlayerTarget->ReleaseRef();
    m_pPlayer->ReleaseRef();
    EZ_DEFAULT_DELETE(m_pWorld);

    ezStartup::ShutdownEngine();
    Cleanup();

    ezPlugin::UnloadPlugin("ezInspectorPlugin");

    ezTelemetry::CloseConnection();
}

ezApplication::ApplicationExecution gpAndyForceFieldsApp::Run()
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

        if (g_bSkipUpdate)
        {
            g_bSkipUpdate = false;
        }
        else
        {
            Update(tUpdateInterval);
        }

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

void gpAndyForceFieldsApp::CreateForceFields()
{
    gpVec3 Positions[] = {
        { 250.0f, 200.0f, 0.0f },
        { 300.0f, 250.0f, 0.0f },
        { 200.0f, 250.0f, 0.0f },
    };

    for (size_t i = 0; i < EZ_ARRAY_SIZE(Positions); ++i)
    {
        auto pForceField = m_pWorld->CreateEntity<gpForceFieldEntity>();
        // Name
        {
            ezStringBuilder sbName;
            sbName.AppendFormat("ForceField#%u", i);
            pForceField->SetName(sbName.GetData());
        }

        pForceField->SetRadius(150.0f);
        pForceField->SetForce(150.0f);

        auto pProps = pForceField->GetProperties();
        pProps->m_Position = Positions[i];

        m_pWorld->AddEntity(pForceField);
    }

    ezLog::Success("Created %u force fields", EZ_ARRAY_SIZE(Positions));
}

void gpAndyForceFieldsApp::CreatePlayer()
{
    m_pPlayer = m_pWorld->CreateEntity<gpParticleEntity>();
    m_pPlayer->AddRef();
    m_pPlayer->SetName("Player");
    auto pProperties = m_pPlayer->GetProperties();
    pProperties->m_Position.Set(100, 200, 0);
    pProperties->m_fGravityFactor = 0.0f;
    //m_pPlayer->SetLinearVelocity(gpVec3(10, 10, 0));
}

ezResult gpAndyForceFieldsApp::BeginSpawningPlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    auto MousePos = GetMousePosition();
    gpRectF SpawnArea; // From bottom left to upper right
    SpawnArea.x = 0.0f;
    SpawnArea.y = static_cast<gpScalar>(gpWindow::GetWidthCVar()->GetValue());
    SpawnArea.width =   static_cast<gpScalar>(g_iPlayerSpawnAreaWidth.GetValue());
    SpawnArea.height = -static_cast<gpScalar>(g_iPlayerSpawnAreaHeight.GetValue());
    if (!gpContains(SpawnArea, MousePos))
        return EZ_FAILURE;

    auto pProps = m_pPlayer->GetProperties();
    pProps->m_Position = MousePos;
    pProps->m_LinearVelocity.SetZero();
    pProps->m_fGravityFactor = 0.0f;

    auto result = m_pWorld->AddEntity(m_pPlayer);
    EZ_ASSERT(result.Succeeded(), "Failed to spawn player");
    auto& DrawInfo = m_pWorld->GetEntityDrawInfo(m_pPlayer);
    DrawInfo.m_Color = ezColor(1, 0, 0, 0.9f);
    DrawInfo.m_fScale = 8.0f;

    ezLog::Success("Player added to the world.");

    return EZ_SUCCESS;
}

void gpAndyForceFieldsApp::FinalizePlayerSpawning()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    auto pProps = m_pPlayer->GetProperties();
    pProps->m_fGravityFactor = 1.0f;
    pProps->m_LinearVelocity = GetMousePosition() - pProps->m_Position;

    auto fMaxSpeed = s_fPlayerMaxSpeed.GetValue();

    if (pProps->m_LinearVelocity.GetLengthSquared() > fMaxSpeed * fMaxSpeed)
    {
        pProps->m_LinearVelocity.SetLength(fMaxSpeed);
    }

    ezLog::Success("Finished player spawning.");
}

void gpAndyForceFieldsApp::DespawnPlayer()
{
    auto result = m_pWorld->RemoveEntity(m_pPlayer);
    EZ_VERIFY(result.Succeeded(), "Failed to despawn player.");
    ezLog::Info("Despawned player.");
}

namespace
{
    enum class PlayerSpawnState
    {
        NotInWorld,
        Spawning,
        Spawned
    };
}

void gpAndyForceFieldsApp::Update(ezTime dt)
{
    static PlayerSpawnState PlayerState = PlayerSpawnState::NotInWorld;
    if(g_StopWatch.GetRunningTotal() > ezTime::Milliseconds(500))
    {
        g_StopWatch.StopAndReset();
        g_SpawnAreaColor.a = 0.1f;
    }

    // If cancel spawning and the player is in the world => despawn player.
    if (ezInputManager::GetInputActionState("Game", "CancelSpawn") == ezKeyState::Pressed)
    {
        switch(PlayerState)
        {
        case PlayerSpawnState::Spawning:
            gpRenderExtractor::RemoveExtractionListener(gpRenderExtractionListener(&gpAndyForceFieldsApp::ExtractVelocityData, this));
        case PlayerSpawnState::Spawned:
            DespawnPlayer();
            DespawnTarget(m_pWorld);

            m_pWorld->ClearWorld();
            CreateForceFields();
            SpawnTarget(m_pWorld);
            m_pWorld->CollectGarbage();

            gpRenderExtractor::AddExtractionListener(ExtractSpawnData);
            break;
        default:
            break;
        }

        PlayerState = PlayerSpawnState::NotInWorld;
    }

    if (ezInputManager::GetInputActionState("Game", "Spawn") == ezKeyState::Pressed)
    {
        switch(PlayerState)
        {
        case PlayerSpawnState::NotInWorld:
            if(BeginSpawningPlayer().Failed())
            {
                ezLog::Info("Failed to spawn player. Make sure to click in the spawn area.");
                g_StopWatch.Resume();
                g_SpawnAreaColor.a *= 2;
                break;
            }
            gpRenderExtractor::RemoveExtractionListener(ExtractSpawnData);
            gpRenderExtractor::AddExtractionListener(gpRenderExtractionListener(&gpAndyForceFieldsApp::ExtractVelocityData, this));
            PlayerState = PlayerSpawnState::Spawning;
            break;
        case PlayerSpawnState::Spawning:
            gpRenderExtractor::RemoveExtractionListener(gpRenderExtractionListener(&gpAndyForceFieldsApp::ExtractVelocityData, this));
            FinalizePlayerSpawning();
            PlayerState = PlayerSpawnState::Spawned;
            break;
        default:
            break;
        }
    }
}

void gpAndyForceFieldsApp::ExtractVelocityData(gpRenderExtractor* pExtractor)
{
    auto pVelVector = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
    pVelVector->m_Start = m_pPlayer->GetProperties()->m_Position;
    pVelVector->m_End.SetZero();
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &pVelVector->m_End.x);
    pVelVector->m_End.x *= gpWindow::GetWidthCVar()->GetValue();

    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &pVelVector->m_End.y);
    pVelVector->m_End.y *= gpWindow::GetHeightCVar()->GetValue();
}

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
#include "gpCore/Shapes/Circle.h"
#include "gpCore/Shapes/Rectangle.h"
#include "gpCore/Task.h"
#include "gpCore/World/RigidBody.h"
#include "gpCore/Utilities/RandomNumbers.h"

namespace
{
    //enum class PlayerSpawnState
    //{
    //    NotInWorld,
    //    Spawning,
    //    Spawned
    //};

    struct PlayerSpawnStateBase
    {
        enum Enum
        {
            NotInWorld,
            Spawning,
            Spawned,

            DefaultInit = NotInWorld
        };

        EZ_FORCE_INLINE static EZ_ENUM_TO_STRING(NotInWorld, Spawning, Spawned);
    };

    using PlayerSpawnState = ezEnum<PlayerSpawnStateBase, ezUInt8>;
}

static PlayerSpawnState PlayerState = PlayerSpawnState::NotInWorld;

ezCVarFloat gpAndyForceFieldsApp::s_fPlayerMaxSpeed("PlayerMaxSpeed", 150.0f, ezCVarFlags::Default, "The maximum speed the player may reach via user input.");

ezCVarInt g_iPlayerSpawnAreaWidth("PlayerSpawnAreaWidth", 150, ezCVarFlags::Default, "Width of the spawn area of the player.");
ezCVarInt g_iPlayerSpawnAreaHeight("PlayerSpawnAreaHeight", 150, ezCVarFlags::Default, "Width of the spawn area of the player.");
ezCVarFloat g_fPlayerTargetRadius("PlayerTargetRadius", 40.0f, ezCVarFlags::Default, "Radius of the target.");

ezColor g_SpawnAreaColor(1.0f, 0.0f, 0.0f, 0.1f);

ezStopwatch g_StopWatch;

gpRigidBody* g_pPlayerTarget = nullptr;
ezDynamicArray<gpForceFieldEntity*> g_ForceFields;

gpRandomNumberGenerator g_Rand;

static void SpawnTarget(gpWorld* pWorld)
{
    EZ_ASSERT(g_pPlayerTarget, "Target not created yet.");

    if (g_pPlayerTarget->GetWorld() != nullptr)
        return;

    // \todo Random position for target.
    g_pPlayerTarget->GetProperties()->m_Position.Set(450, 50, 0);

    pWorld->AddEntity(g_pPlayerTarget);
    pWorld->GetEntityDrawInfo(g_pPlayerTarget).m_Color = ezColor::GetYellow();

    static_cast<gpCircleShape*>(g_pPlayerTarget->GetShape())->SetRadius(g_fPlayerTargetRadius.GetValue());

    ezLog::Success("Target spawned.");
}

static void DespawnTarget(gpWorld* pWorld)
{
    if (g_pPlayerTarget->GetWorld() != pWorld)
        return;

    pWorld->RemoveEntity(g_pPlayerTarget);
    ezLog::Info("Target despawned.");
}

static void CreateTarget(gpWorld* pWorld)
{
    static gpCircleShape* pCircle;

    if(pCircle == nullptr)
    {
        static gpCircleShape Circle;
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

static void EnableSpawnDataExtraction(bool bEnabled)
{
    static bool bIsExtracting = false;

    if(bEnabled && !bIsExtracting)
    {
        gpRenderExtractor::AddExtractionListener(ExtractSpawnData);
        bIsExtracting = true;
    }
    else if(!bEnabled && bIsExtracting)
    {
        gpRenderExtractor::RemoveExtractionListener(ExtractSpawnData);
        bIsExtracting = false;
    }
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
        RegisterInputAction("Game", "Reset", ezInputSlot_KeySpace);
        // Poll once to finish input initialization
        ezInputManager::PollHardware();
        SetupRendering();
    }

    m_pWorld = EZ_DEFAULT_NEW(gpWorld)("PrimaryWorld");
    //m_pWorld->SetGravity(gpVec3(0, 9.81f, 0));
    gpRenderExtractor::AddExtractionListener(
        gpRenderExtractionListener(&gpWorld::ExtractRenderingData, m_pWorld));
    EnableSpawnDataExtraction(true);
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
    static ezUInt32 uiInstanceCount = 0;

    static ezCVarFloat fMinX("ForceFieldMinPosX", 190.0f, ezCVarFlags::Default, "");
    static ezCVarFloat fMinY("ForceFieldMinPosY", 110.0f, ezCVarFlags::Default, "");
    static ezCVarFloat fMaxX("ForceFieldMaxPosX", 400.0f, ezCVarFlags::Default, "");
    static ezCVarFloat fMaxY("ForceFieldMaxPosY", 330.0f, ezCVarFlags::Default, "");

    gpVec3 MinPosition(fMinX.GetValue(), fMinY.GetValue(), 0.0f);
    gpVec3 MaxPosition(fMaxX.GetValue(), fMaxY.GetValue(), 0.0f);

    static gpScalar MinRadius = 100.0f;
    static gpScalar MaxRadius = 150.0f;

    static gpScalar MinForce = 50.0f;
    static gpScalar MaxForce = 200.0f;

    const auto uiNumForceFields = g_Rand.GenerateInteger<ezUInt64>(2, 3);

    for (ezUInt64 i = 0; i < uiNumForceFields; ++i)
    {
        auto pForceField = m_pWorld->CreateEntity<gpForceFieldEntity>();
        // Name
        {
            ezStringBuilder sbName;
            sbName.AppendFormat("ForceField#%u", uiInstanceCount++);
            pForceField->SetName(sbName.GetData());
        }

        pForceField->SetRadius(g_Rand.GenerateFloat(MinRadius, MaxRadius));
        pForceField->SetForce(g_Rand.GenerateFloat(MinForce, MinForce));

        auto pProps = pForceField->GetProperties();
        gpRandomize(g_Rand, pProps->m_Position, MinPosition, MaxPosition);

        m_pWorld->AddEntity(pForceField);
    }

    ezLog::Success("Created %u force fields", uiNumForceFields);

    // Debugging:
    static bool bDrawSpawnBounds = true;
    if(bDrawSpawnBounds)
    {
        bDrawSpawnBounds = false;

        gpRenderExtractor::AddExtractionListener([](gpRenderExtractor* pExtractor){
            auto pArea = pExtractor->AllocateRenderData<gpDrawData::Box>();
            pArea->m_Box.x       = (ezUInt32)fMinX.GetValue();
            pArea->m_Box.y       = (ezUInt32)fMinY.GetValue();
            pArea->m_Box.width   = (ezUInt32)fMaxX.GetValue() - pArea->m_Box.x;
            pArea->m_Box.height  = (ezUInt32)fMaxY.GetValue() - pArea->m_Box.y;
            pArea->m_FillColor   = ezColor::GetGreen();
            pArea->m_FillColor.a = 0.1f;
        });
    }
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

bool gpAndyForceFieldsApp::CanSpawnPlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    auto MousePos = GetMousePosition();
    gpRectF SpawnArea; // From bottom left to upper right
    SpawnArea.x = 0.0f;
    SpawnArea.y = static_cast<gpScalar>(gpWindow::GetWidthCVar()->GetValue());
    SpawnArea.width =   static_cast<gpScalar>(g_iPlayerSpawnAreaWidth.GetValue());
    SpawnArea.height = -static_cast<gpScalar>(g_iPlayerSpawnAreaHeight.GetValue());

    return gpContains(SpawnArea, MousePos);
}

void gpAndyForceFieldsApp::SpawnAndFreezePlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    if(m_pPlayer->GetWorld() != nullptr)
        return; // Player is already spawned.

    auto pProps = m_pPlayer->GetProperties();
    pProps->m_Position = GetMousePosition();
    pProps->m_LinearVelocity.SetZero();
    pProps->m_fGravityFactor = 0.0f;

    auto result = m_pWorld->AddEntity(m_pPlayer);
    EZ_ASSERT(result.Succeeded(), "Failed to spawn player");
    auto& DrawInfo = m_pWorld->GetEntityDrawInfo(m_pPlayer);
    DrawInfo.m_Color = ezColor(1, 0, 0, 0.9f);
    DrawInfo.m_fScale = 8.0f;

    ezLog::Success("Player added to the world.");
}

void gpAndyForceFieldsApp::UnfreezePlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    auto pProps = m_pPlayer->GetProperties();

    if(!ezMath::IsZero(pProps->m_LinearVelocity.GetLengthSquared(), 0.01f))
        return; // Player is already moving

    pProps->m_fGravityFactor = 1.0f;
    pProps->m_LinearVelocity = GetMousePosition() - pProps->m_Position;

    auto fMaxSpeed = s_fPlayerMaxSpeed.GetValue();

    if (pProps->m_LinearVelocity.GetLengthSquared() > fMaxSpeed * fMaxSpeed)
    {
        pProps->m_LinearVelocity.SetLength(fMaxSpeed);
    }

    ezLog::Success("Player is moving.");
}

void gpAndyForceFieldsApp::DespawnPlayer()
{
    EZ_ASSERT(m_pPlayer, "Player not created.");

    if(m_pPlayer->GetWorld() == nullptr)
        return;

    auto result = m_pWorld->RemoveEntity(m_pPlayer);
    EZ_VERIFY(result.Succeeded(), "Failed to despawn player.");
    ezLog::Info("Despawned player.");
}

void gpAndyForceFieldsApp::Update(ezTime dt)
{
    if(g_StopWatch.GetRunningTotal() > ezTime::Milliseconds(500))
    {
        g_StopWatch.StopAndReset();
        g_SpawnAreaColor.a = 0.1f;
    }

    // If cancel spawning and the player is in the world => despawn player.
    if (ezInputManager::GetInputActionState("Game", "CancelSpawn") == ezKeyState::Pressed)
    {
        PlayerState = PlayerSpawnState::NotInWorld;
    }

    if (ezInputManager::GetInputActionState("Game", "Reset") == ezKeyState::Pressed)
    {
        ResetWorld();
        PlayerState = PlayerSpawnState::NotInWorld;
    }

    if (ezInputManager::GetInputActionState("Game", "Spawn") == ezKeyState::Pressed)
    {
        switch(PlayerState)
        {
        case PlayerSpawnState::NotInWorld:
            if(CanSpawnPlayer())
            {
                PlayerState = PlayerSpawnState::Spawning;
                break;
            }

            ezLog::Info("Failed to spawn player. Make sure to click in the spawn area.");
            g_StopWatch.Resume();
            g_SpawnAreaColor.a *= 2;
            break;
        case PlayerSpawnState::Spawning:
            PlayerState = PlayerSpawnState::Spawned;
            break;
        default:
            break;
        }
    }
    else if (PlayerState != PlayerSpawnState::NotInWorld)
    {
        auto& Pos = m_pPlayer->GetProperties()->m_Position;
        if (Pos.x < 0.0f || Pos.x > gpWindow::GetWidthCVar()->GetValue()
         || Pos.y < 0.0f || Pos.y > gpWindow::GetHeightCVar()->GetValue())
        {
            PlayerState = PlayerSpawnState::NotInWorld;
            ezLog::Info("Player left the world");
        }

        if (gpContains(g_pPlayerTarget->GetProperties(), *static_cast<gpCircleShape*>(g_pPlayerTarget->GetShape()),
                       m_pPlayer->GetProperties()->m_Position))
        {
            ezLog::Info("Player hit the target!");
            ResetWorld();
            PlayerState = PlayerSpawnState::NotInWorld;
        }
    }

    EZ_LOG_BLOCK("Update", PlayerSpawnState::ToString(PlayerState));

    switch(PlayerState)
    {
    case PlayerSpawnState::NotInWorld:
        DespawnPlayer();
        EnableVelocityDataExtraction(false);
        EnableSpawnDataExtraction(true);
        break;
    case PlayerSpawnState::Spawning:
        SpawnAndFreezePlayer();
        EnableVelocityDataExtraction(true);
        EnableSpawnDataExtraction(false);
        break;
    case PlayerSpawnState::Spawned:
        UnfreezePlayer();
        EnableVelocityDataExtraction(false);
        EnableSpawnDataExtraction(false);
        break;
    default:
        EZ_REPORT_FAILURE("Should never reach this code.");
        break;
    }
}

void gpAndyForceFieldsApp::ResetWorld()
{
    DespawnPlayer();
    DespawnTarget(m_pWorld);

    m_pWorld->ClearWorld();

    CreateForceFields();
    SpawnTarget(m_pWorld);

    m_pWorld->CollectGarbage();
    EnableSpawnDataExtraction(true);
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

void gpAndyForceFieldsApp::EnableVelocityDataExtraction(bool bEnabled)
{
    static bool bIsExtracting = false;

    if (bEnabled && !bIsExtracting)
    {
        gpRenderExtractor::AddExtractionListener(gpRenderExtractionListener(&gpAndyForceFieldsApp::ExtractVelocityData, this));
        bIsExtracting = true;
    }
    else if(!bEnabled && bIsExtracting)
    {
        gpRenderExtractor::RemoveExtractionListener(gpRenderExtractionListener(&gpAndyForceFieldsApp::ExtractVelocityData, this));
        bIsExtracting = false;
    }
}

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
#include "gpCore/World/ForceField.h"
#include "gpCore/Shapes/Circle.h"
#include "gpCore/Shapes/Rectangle.h"
#include "gpCore/Task.h"
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

ezCVarFloat gpAndyForceFieldsApp::s_fPlayerMaxSpeed("Player/MaxSpeed", 150.0f, ezCVarFlags::Default, "The maximum speed the player may reach via user input.");

static ezCVarInt g_iPlayerSpawnAreaWidth("Player/SpawnArea/Width", 150, ezCVarFlags::Default, "Width of the spawn area of the player.");
static ezCVarInt g_iPlayerSpawnAreaHeight("Player/SpawnArea/Height", 150, ezCVarFlags::Default, "Width of the spawn area of the player.");
static ezCVarFloat g_fPlayerTargetRadius("PlayerTarget/Radius", 40.0f, ezCVarFlags::Default, "Radius of the target.");

static ezCVarFloat g_fForceFieldMinPosX("ForceField/MinPosX", 190.0f, ezCVarFlags::Default, "The minimum x value a force field can be positioned at.");
static ezCVarFloat g_fForceFieldMinPosY("ForceField/MinPosY", 110.0f, ezCVarFlags::Default, "The minimum y value a force field can be positioned at.");
static ezCVarFloat g_fForceFieldMaxPosX("ForceField/MaxPosX", 400.0f, ezCVarFlags::Default, "The maximum x value a force field can be positioned at.");
static ezCVarFloat g_fForceFieldMaxPosY("ForceField/MaxPosY", 330.0f, ezCVarFlags::Default, "The maximum y value a force field can be positioned at.");

static ezCVarFloat g_fForceFieldMinRadius("ForceField/MinRadius", 100.0f, ezCVarFlags::Default, "The minimum radius of a force field.");
static ezCVarFloat g_fForceFieldMaxRadius("ForceField/MaxRadius", 150.0f, ezCVarFlags::Default, "The minimum radius of a force field.");

static ezCVarFloat g_fForceFieldMinForce("ForceField/MinForce", 50.0f, ezCVarFlags::Default, "The minimum force of a force field.");
static ezCVarFloat g_fForceFieldMaxForce("ForceField/MaxForce", 200.0f, ezCVarFlags::Default, "The minimum force of a force field.");

static ezCVarBool g_bForceFieldDrawSpawnArea("ForceField/DrawSpawnArea", false, ezCVarFlags::Default, "Whether the spawn area of force fields should be rendered.");

ezColor g_SpawnAreaColor(1.0f, 0.0f, 0.0f, 0.1f);

ezStopwatch g_StopWatch;

gpEntity* g_pPlayerTarget = nullptr;
#define playerTarget Deref(g_pPlayerTarget)

ezDynamicArray<gpForceFieldEntity*> g_ForceFields;

gpRandomNumberGenerator g_Rand;

// Easy access
#define player Deref(m_pPlayer)

#define world Deref(m_pWorld)

static void SpawnTarget(gpWorld* pWorld)
{
    EZ_ASSERT(g_pPlayerTarget, "Target not created yet.");

    if (gpWorldPtrOf(playerTarget) != nullptr)
        return;

    // \todo Random position for target.
    gpSet(gpPositionOf(playerTarget), 450, 50, 0);

    gpAddTo(Deref(pWorld), playerTarget);
    gpDrawInfoOf(Deref(pWorld), playerTarget).m_Color = ezColor::GetYellow();

    gpRadiusOf(static_cast<gpCircleShape&>(Deref(gpShapePtrOf(playerTarget)))) = g_fPlayerTargetRadius.GetValue();

    ezLog::Success("Target spawned.");
}

static void DespawnTarget(gpWorld* pWorld)
{
    if (gpWorldPtrOf(playerTarget) != pWorld)
        return;

    gpRemoveFrom(Deref(pWorld), playerTarget);
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

    g_pPlayerTarget = gpNew<gpEntity>(Deref(pWorld));
    EZ_ASSERT(g_pPlayerTarget, "Failed to create player target (rigid body)");

    gpAddReferenceTo(playerTarget);
    gpNameOf(playerTarget) = "Target";
    gpShapePtrOf(playerTarget) = pCircle;
    gpGravityFactorOf(playerTarget) = 0.0f;

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

static void ExtractForceFieldSpawnArea(gpRenderExtractor* pExtractor)
{
    auto pArea = pExtractor->AllocateRenderData<gpDrawData::Box>();
    pArea->m_Box.x       = (ezUInt32)g_fForceFieldMinPosX.GetValue();
    pArea->m_Box.y       = (ezUInt32)g_fForceFieldMinPosY.GetValue();
    pArea->m_Box.width   = (ezUInt32)g_fForceFieldMaxPosX.GetValue() - pArea->m_Box.x;
    pArea->m_Box.height  = (ezUInt32)g_fForceFieldMaxPosY.GetValue() - pArea->m_Box.y;
    pArea->m_FillColor   = ezColor::GetGreen();
    pArea->m_FillColor.a = 0.1f;
}

static void EnableForceFieldSpawnAreaExtraction(bool bEnabled)
{
    static bool bIsExtracting = false;

    if(bEnabled && !bIsExtracting)
    {
        gpRenderExtractor::AddExtractionListener(ExtractForceFieldSpawnArea);
        bIsExtracting = true;
    }
    else if(!bEnabled && bIsExtracting)
    {
        gpRenderExtractor::RemoveExtractionListener(ExtractForceFieldSpawnArea);
        bIsExtracting = false;
    }
}

enum gpPositionOfOverloadHelper { MouseCursor };

// Call like this: gpPositionOf(MouseCursor)
static gpDisplacement gpPositionOf(gpPositionOfOverloadHelper)
{
    float fX;
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionX, &fX);
    fX *= gpWindow::GetWidthCVar()->GetValue();

    float fY;
    ezInputManager::GetInputSlotState(ezInputSlot_MousePositionY, &fY);
    fY *= gpWindow::GetHeightCVar()->GetValue();

    return gpDisplacement(fX, fY, 0.0f);
}

static bool g_bSkipUpdate = false;

void gpAndyForceFieldsApp::AfterEngineInit()
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
        RegisterInputAction("Game", "Spawn", ezInputSlot_MouseButton0);
        RegisterInputAction("Game", "CancelSpawn", ezInputSlot_MouseButton1);
        RegisterInputAction("Game", "Reset", ezInputSlot_KeySpace);
        // Poll once to finish input initialization
        ezInputManager::PollHardware();
        SetupRendering();
    }

    m_pWorld = EZ_DEFAULT_NEW(gpWorld)("PhysicsWorld");
    //m_pWorld->SetGravity(gpVec3(0, 9.81f, 0));
    gpRenderExtractor::AddExtractionListener([=](gpRenderExtractor* pExtractor){
        gpExtractRenderDataOf(world, pExtractor);
    });
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

    g_bForceFieldDrawSpawnArea.m_CVarEvents.AddEventHandler([](const ezCVar::CVarEvent& Data)
    {
        if(Data.m_EventType != ezCVar::CVarEvent::ValueChanged)
            return;
        EnableForceFieldSpawnAreaExtraction(g_bForceFieldDrawSpawnArea.GetValue());
    });
}

void gpAndyForceFieldsApp::BeforeEngineShutdown()
{
    gpReleaseReferenceTo(playerTarget);
    gpReleaseReferenceTo(player);
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

        gpStepSimulationOf(world, tUpdateInterval);

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

    gpVec3 MinPosition(g_fForceFieldMinPosX.GetValue(), g_fForceFieldMinPosY.GetValue(), 0.0f);
    gpVec3 MaxPosition(g_fForceFieldMaxPosX.GetValue(), g_fForceFieldMaxPosY.GetValue(), 0.0f);

    const auto uiNumForceFields = g_Rand.GenerateInteger<ezUInt64>(2, 3);

    for (ezUInt64 i = 0; i < uiNumForceFields; ++i)
    {
        auto pForceField = gpCreateEntity<gpForceFieldEntity>(world);
        EZ_ASSERT(pForceField, "Failed to create force field.");

        auto& forceField = Deref(pForceField);

        // Name
        {
            ezStringBuilder sbName;
            sbName.AppendFormat("ForceField#%u", uiInstanceCount++);
            gpNameOf(forceField) = sbName;
        }

        gpRadiusOf(forceField) = g_Rand.GenerateFloat(g_fForceFieldMinRadius.GetValue(), g_fForceFieldMaxRadius.GetValue());
        gpForceFactorOf(forceField) = g_Rand.GenerateFloat(g_fForceFieldMinForce.GetValue(), g_fForceFieldMaxForce.GetValue());

        gpRandomize(g_Rand, gpValueOf(gpPositionOf(forceField)), MinPosition, MaxPosition);

        gpAddTo(world, forceField);
    }

    ezLog::Success("Created %u force fields", uiNumForceFields);
}

void gpAndyForceFieldsApp::CreatePlayer()
{
    m_pPlayer = gpNew<gpEntity>(world);
    EZ_ASSERT(m_pPlayer, "Failed to create player (particle)");

    gpAddReferenceTo(player);
    gpNameOf(player) = "Player";
    gpSet(gpPositionOf(player), 100, 200, 0);
    gpGravityFactorOf(player) = 0.0f;
    //gpSet(gpLinearVelocityOf(player), /* => */ 10, 10, 0);
}

bool gpAndyForceFieldsApp::CanSpawnPlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    gpRectF SpawnArea; // From bottom left to upper right
    SpawnArea.x = 0.0f;
    SpawnArea.y = static_cast<gpScalar>(gpWindow::GetWidthCVar()->GetValue());
    SpawnArea.width =   static_cast<gpScalar>(g_iPlayerSpawnAreaWidth.GetValue());
    SpawnArea.height = -static_cast<gpScalar>(g_iPlayerSpawnAreaHeight.GetValue());

    return gpContains(SpawnArea, gpPositionOf(MouseCursor));
}

void gpAndyForceFieldsApp::SpawnAndFreezePlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    if(gpWorldPtrOf(player) != nullptr)
        return; // Player is already spawned.

    gpPositionOf(player) = gpPositionOf(MouseCursor);
    gpLinearVelocityOf(player) = gpLinearVelocity(gpVec3::ZeroVector());
    gpGravityFactorOf(player) = 0.0f;

    auto result = gpAddTo(world, player);
    EZ_ASSERT(result.Succeeded(), "Failed to spawn player");
    auto& DrawInfo = gpDrawInfoOf(world, player);
    DrawInfo.m_Color = ezColor(1, 0, 0, 0.9f);
    DrawInfo.m_fScale = 8.0f;

    ezLog::Success("Player added to the world.");
}

void gpAndyForceFieldsApp::UnfreezePlayer()
{
    EZ_ASSERT(m_pPlayer, "Not initialized.");

    if(!ezMath::IsZero(gpValueOf(gpLinearVelocityOf(player)).GetLengthSquared(), 0.01f))
        return; // Player is already moving

    gpGravityFactorOf(player) = 1.0f;
    gpLinearVelocityOf(player) = gpLinearVelocity(gpValueOf(gpPositionOf(MouseCursor) - gpPositionOf(player)));

    auto fMaxSpeed = s_fPlayerMaxSpeed.GetValue();

    if (gpValueOf(gpLinearVelocityOf(player)).GetLengthSquared() > fMaxSpeed * fMaxSpeed)
    {
        auto v = gpValueOf(gpLinearVelocityOf(player));
        v.SetLength(fMaxSpeed);
        gpLinearVelocityOf(player) = gpLinearVelocity(v);
    }

    ezLog::Success("Player is moving.");
}

void gpAndyForceFieldsApp::DespawnPlayer()
{
    EZ_ASSERT(m_pPlayer, "Player not created.");

    if(gpWorldPtrOf(player) == nullptr)
        return;

    auto result = gpRemoveFrom(Deref(m_pWorld), player);
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
        const auto& Pos = gpValueOf(gpPositionOf(player));
        if (Pos.x < 0.0f || Pos.x > gpWindow::GetWidthCVar()->GetValue()
         || Pos.y < 0.0f || Pos.y > gpWindow::GetHeightCVar()->GetValue())
        {
            PlayerState = PlayerSpawnState::NotInWorld;
            ezLog::Info("Player left the world");
        }

        if (gpContains(gpPhysicalPropertiesOf(playerTarget),
                       static_cast<gpCircleShape&>(Deref(gpShapePtrOf(playerTarget))),
                       gpPositionOf(player)))
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

    gpClear(world);

    CreateForceFields();
    SpawnTarget(m_pWorld);

    gpCollectGarbageOf(world);
    EnableSpawnDataExtraction(true);
}

void gpAndyForceFieldsApp::ExtractVelocityData(gpRenderExtractor* pExtractor)
{
    auto pVelVector = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
    pVelVector->m_Start = gpValueOf(gpPositionOf(player));
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

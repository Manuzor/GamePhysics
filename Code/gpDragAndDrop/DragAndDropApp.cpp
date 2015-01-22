#include "gpDragAndDrop/PCH.h"

#include <Core/Input/InputManager.h>

#include "gpDragAndDrop/DragAndDropApp.h"

#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"

#include "gpCore/World/World.h"
#include "gpCore/Shapes.h"

#include "gpCore/Utilities/RandomNumbers.h"

static gpWorld* g_pWorld = nullptr;
static ezDynamicArray<gpEntity*> g_entities;

static const gpScalar g_defaultDamping = 0.001f;

static gpRandomNumberGenerator g_rand;

static gpEntity* Sphere(const char* name, const gpDisplacement& position)
{
    auto pParticle = gpNew<gpEntity>();
    g_entities.PushBack(pParticle);

    auto& p = Deref(pParticle);

    gpNameOf(p)          = name;
    gpPositionOf(p)      = position;
    gpMassOf(p)          = gpMass((gpScalar)g_rand.GenerateInteger(5, 100));
    gpLinearDampingOf(p) = g_defaultDamping;
    gpShapePtrOf(p)      = gpNew<gpSphereShape>(gpValueOf(gpMassOf(p)) * 0.2f);

    return pParticle;
}

static void Populate(gpWorld& world)
{
    gpGravityOf(world) = gpLinearAcceleration(gpVec3(0, 0, 0));

    auto pShape        = gpNew<gpSphereShape>(20.0f);
    gpScalar offset    = 90.0f;
    gpScalar padding   = 75.0f;
    ezStringBuilder name;
    for (ezUInt32 x = 0; x < 5; ++x)
    {
        for (ezUInt32 y = 0; y < 5; ++y)
        {
            name.Format("particle %d, %d", x, y);
            gpDisplacement d(padding * x + offset, padding * y + offset, 0);
            auto p = Sphere(name.GetData(), d);
            gpAddTo(world, Deref(p));
        }
    }
}

static void Cleanup()
{
    gpDelete(g_pWorld);
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

static bool IsUnderMouseCursor(const gpDisplacement& pos, gpScalar radius)
{
    auto diff = gpPositionOf(MouseCursor) - pos;

    return gpIsZero(diff) || gpSquaredLengthOf(diff) < gpSquare(radius);
}

static gpEntity* FindEntityBelowMouseCursor()
{
    for (ezUInt32 i = 0; i < g_entities.GetCount(); ++i)
    {
        auto pEntity = g_entities[i];
        auto& entity = Deref(pEntity);
        auto& shape = gpShapeOf(entity);
        if (IsUnderMouseCursor(gpPositionOf(entity), gpRadiusOf(shape)))
        {
            return pEntity;
        }
    }

    return nullptr;
}

static void Update(gpTime dt)
{
    static bool s_simulationPaused = false;
    static gpEntity* s_pDraggedObject = nullptr;

    bool forceStep = ezInputManager::GetInputActionState("Game", "Step") == ezKeyState::Pressed;

    if(ezInputManager::GetInputActionState("Game", "Pause") == ezKeyState::Pressed)
    {
        s_simulationPaused = !s_simulationPaused;
        if (s_simulationPaused)
        {
            ezLog::Info("Pausing simulation.");
        }
        else
        {
            ezLog::Info("Resuming simulation.");
        }
    }

    if (s_simulationPaused && !forceStep)
        return;

    auto dragState = ezInputManager::GetInputActionState("Game", "Drag");

    if (dragState == ezKeyState::Pressed)
    {
        s_pDraggedObject = FindEntityBelowMouseCursor();
        if (s_pDraggedObject)
        {
            gpLinearDampingOf(Deref(s_pDraggedObject)) = 0.1f;
        }
    }
    else if (dragState == ezKeyState::Up)
    {
        if (s_pDraggedObject)
        {
            gpLinearDampingOf(Deref(s_pDraggedObject)) = g_defaultDamping;
        }
        s_pDraggedObject = nullptr;
    }

    if (s_pDraggedObject && dragState == ezKeyState::Down)
    {
        auto diff = gpValueOf(gpPositionOf(MouseCursor) - gpPositionOf(Deref(s_pDraggedObject)));
        gpApplyLinearImpulseTo(Deref(s_pDraggedObject), gpLinearVelocity(diff * 2.0f));
    }

    gpStepSimulationOf(Deref(g_pWorld), dt);
}

void gpDragAndDropApp::AfterEngineInit()
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
        RegisterInputAction("Game", "Drag", ezInputSlot_MouseButton0);
        RegisterInputAction("Game", "Reset", ezInputSlot_MouseButton1);
        RegisterInputAction("Game", "Pause", ezInputSlot_KeySpace);
        RegisterInputAction("Game", "Step", ezInputSlot_KeyReturn);
        // Poll once to finish input initialization;
        ezInputManager::PollHardware();
        SetupRendering();
    }

    gpRenderExtractor::AddExtractionListener([](gpRenderExtractor* pExtractor){
        gpExtractRenderDataOf(Deref(g_pWorld), pExtractor);
    });

    g_pWorld = gpNew<gpWorld>("World");
    Populate(Deref(g_pWorld));
}

void gpDragAndDropApp::BeforeEngineShutdown()
{
    ::Cleanup();

    ezStartup::ShutdownEngine();
    Cleanup();

    ezPlugin::UnloadPlugin("ezInspectorPlugin");
    ezTelemetry::CloseConnection();
}

ezApplication::ApplicationExecution gpDragAndDropApp::Run()
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

#include "gpDragAndDrop/PCH.h"

#include <Core/Input/InputManager.h>

#include "gpDragAndDrop/Application.h"

#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"

#include "gpCore/World/World.h"
#include "gpCore/Shapes.h"

static gpWorld* g_pWorld = nullptr;
static gpEntity* g_pSphere = nullptr;
#define sphere Deref(g_pSphere)

static void Populate(gpWorld& world)
{
    gpGravityOf(world) = gpLinearAcceleration(gpVec3(0, 0, 0));

    g_pSphere = gpNew<gpEntity>();

    gpNameOf(sphere)     = "The Sphere";
    gpPositionOf(sphere) = gpDisplacement(250, 250, 0);
    gpMassOf(sphere)     = gpMass(1.0f);
    gpShapePtrOf(sphere) = gpNew<gpSphereShape>(10.0f);

    gpAddTo(world, sphere);
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

static void Update(gpTime dt)
{
    static bool dragginEnabled = false;
    auto dragState = ezInputManager::GetInputActionState("Game", "Drag");

    if (dragState == ezKeyState::Pressed)
    {
        // Find out if the mouse is over the sphere.
        auto diff = gpValueOf(gpPositionOf(MouseCursor) - gpPositionOf(sphere));
        dragginEnabled = gpIsZero(diff) || diff.GetLengthSquared() < gpSquare(gpRadiusOf(gpShapeOf(sphere)));
    }
    else if (dragState == ezKeyState::Up)
    {
        dragginEnabled = false;
    }

    if (dragginEnabled && dragState == ezKeyState::Down)
    {
        auto diff = gpValueOf(gpPositionOf(MouseCursor) - gpPositionOf(sphere));
        gpApplyLinearImpulseTo(sphere, gpLinearVelocity(diff * 1.0f));
    }
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
        auto state = ezInputManager::GetInputActionState("Game", "Pause");
        switch(state)
        {
        case ezKeyState::Released:
            ezLog::Info("Resuming simulation");
        case ezKeyState::Up:
            Update(tUpdateInterval);
            gpStepSimulationOf(Deref(g_pWorld), tUpdateInterval);
            break;
        case ezKeyState::Pressed:
            ezLog::Info("Pausing simulation");
            break;
        case ezKeyState::Down: break;
        default:
            EZ_REPORT_FAILURE("Invalid state.");
            break;
        }

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

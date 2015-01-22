#include "gpRotationSample/PCH.h"

#include <Core/Input/InputManager.h>

#include "gpRotationSample/RotationApp.h"

#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"

#include "gpCore/World/World.h"
#include "gpCore/Shapes.h"

static gpWorld* g_pWorld = nullptr;
ezDynamicArray<gpEntity*> g_entities;

static void Populate(gpWorld& world)
{
    gpGravityOf(world) = gpLinearAcceleration(gpVec3(0, 0, 0));

    gpScalar left    = 100.0f;
    gpScalar padding = 100.0f;
    gpScalar offset  = 150.0f;

    struct MassRadiusPair
    {
        gpScalar r;
        gpMass m;
    };

    struct Row
    {
        MassRadiusPair data[3];
    };

    ezStringBuilder info;
    Row data[3];
    for (int i = 0; i < 3; ++i)
    {
        info.AppendFormat("%d ", i);
        auto& row = data[i];
        for (int j = 0; j < 3; ++j)
        {
            auto& data = row.data[j];
            data.r = 25.0f * (j+1);
            data.m = gpMass(5.0f * (j + 1));
            info.AppendFormat("| (%f/%f) ", gpValueOf(data.m), data.r);
        }
        info.AppendFormat("|\n");
    }

    {
        EZ_LOG_BLOCK("Data distribution", "mass/radius");
        ezLog::Info("%s", info.GetData());
    }

    gpVec3 triangleVertices[] = {
        {  0,  1, 0 },
        { -1, -1, 0 },
        {  1, -1, 0 },
    };
    gpVec3 boxVertices[] = {
        { -1,  1, 0 },
        { -1, -1, 0 },
        {  1, -1, 0 },
        {  1,  1, 0 },
    };
    gpVec3 ocatgonVertices[] = {
        { -1,  1, 0 },
        { -1,  0, 0 },
        { -1, -1, 0 },
        {  0, -1, 0 },
        {  1, -1, 0 },
        {  1,  0, 0 },
        {  1,  1, 0 },
        {  0,  1, 0 },
    };

    // First row: triangles
    for (int i = 0; i < 3; ++i)
    {
        auto pTriangle = gpNew<gpEntity>();
        auto& tri = Deref(pTriangle);

        auto r = data[0].data[i].r;
        auto m = data[0].data[i].m;

        ezStringBuilder name;
        name.AppendFormat("Triangle %d", i);
        gpNameOf(tri) = name;
        gpPositionOf(tri) = gpDisplacement(left + i * offset, padding, 0);
        gpMassOf(tri) = m;
        gpInverseInertiaOf(tri) = gpInverseInertia::SolidCuboid(m, gpVec3(2 * r, 2 * r, 0));
        for (auto& v : triangleVertices)
            v = v.GetNormalized() * r;
        gpShapePtrOf(tri) = gpNew<gpPolygonShape>(triangleVertices);

        gpAddTo(world, tri);
        g_entities.PushBack(pTriangle);
    }

    // Second row: boxes
    for (int i = 0; i < 3; ++i)
    {
        auto pBox = gpNew<gpEntity>();
        auto& box = Deref(pBox);

        auto m = data[0].data[i].m;
        auto r = data[0].data[i].r;

        ezStringBuilder name;
        name.AppendFormat("Box %d", i);
        gpNameOf(box) = name;
        gpPositionOf(box) = gpDisplacement(left + i * offset, padding + offset, 0);
        gpMassOf(box) = m;
        gpInverseInertiaOf(box) = gpInverseInertia::SolidCuboid(m, gpVec3(2 * r, 2 * r, 0));
        for (auto& v : boxVertices)
            v = v.GetNormalized() * r;
        gpShapePtrOf(box) = gpNew<gpPolygonShape>(boxVertices);

        gpAddTo(world, box);
        g_entities.PushBack(pBox);
    }

    // Third row: octagons
    for (int i = 0; i < 3; ++i)
    {
        auto pOctagon = gpNew<gpEntity>();
        auto& oct = Deref(pOctagon);

        auto m = data[0].data[i].m;
        auto r = data[0].data[i].r;

        ezStringBuilder name;
        name.AppendFormat("Octagon %d", i);
        gpNameOf(oct) = name;
        gpPositionOf(oct) = gpDisplacement(left + i * offset, padding + offset * 2, 0);
        gpMassOf(oct) = m;
        gpInverseInertiaOf(oct) = gpInverseInertia::SolidCuboid(m, gpVec3(2 * r, 2 * r, 0));
        for (auto& v : ocatgonVertices)
            v = v.GetNormalized() * r;
        gpShapePtrOf(oct) = gpNew<gpPolygonShape>(ocatgonVertices);

        gpAddTo(world, oct);
        g_entities.PushBack(pOctagon);
    }
}

static void Cleanup()
{
    gpDelete(g_pWorld);
}

static void Update(gpTime dt)
{
    static bool s_simulationPaused = false;

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

    bool forceStep = ezInputManager::GetInputActionState("Game", "Step") == ezKeyState::Pressed;

    if (s_simulationPaused && !forceStep)
        return;

    if (ezInputManager::GetInputActionState("Game", "ApplyLinearImpulse") == ezKeyState::Pressed)
    {
        auto f = gpForce(5000.0f, 0, 0);
        auto p = gpDisplacement(0, 50, 0);
        ezLog::Info("Applying force of { %f, %f, %f } over %f seconds at local { %f, %f, %f }.",
                    gpX(f), gpY(f), gpZ(f),
                    dt,
                    gpX(p), gpY(p), gpZ(p));
        for (ezUInt32 i = 0; i < g_entities.GetCount(); ++i)
        {
            auto& entity = Deref(g_entities[i]);
            gpApplyForceTo(entity, f, dt, p);
        }
    }

    if (ezInputManager::GetInputActionState("Game", "Reset") == ezKeyState::Pressed)
    {
        ezLog::Info("Resetting entities.");
        for (ezUInt32 i = 0; i < g_entities.GetCount(); ++i)
        {
            auto& entity = Deref(g_entities[i]);
            gpAngularMomentumOf(entity) = gpAngularMomentum(gpIdentity);
            gpRotationOf(entity) = gpOrientation(gpIdentity);
        }
    }

    gpStepSimulationOf(Deref(g_pWorld), dt);
}

void gpRotationSampleApp::AfterEngineInit()
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
        RegisterInputAction("Game", "ApplyLinearImpulse", ezInputSlot_Key1);
        RegisterInputAction("Game", "Reset", ezInputSlot_KeyBackspace);
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

void gpRotationSampleApp::BeforeEngineShutdown()
{
    ::Cleanup();

    ezStartup::ShutdownEngine();
    Cleanup();

    ezPlugin::UnloadPlugin("ezInspectorPlugin");
    ezTelemetry::CloseConnection();
}

ezApplication::ApplicationExecution gpRotationSampleApp::Run()
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

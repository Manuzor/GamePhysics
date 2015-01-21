#include "gpExperiments/PCH.h"

#include <Core/Input/InputManager.h>

#include "gpExperiments/Application.h"

#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"

#include "gpCore/World/World.h"
#include "gpCore/Shapes/Shape.h"

static gpWorld* g_pWorld = nullptr;

static void RotatingRigidBody(const char* name, gpWorld& world)
{
    auto pEntity = gpNew<gpEntity>();
    auto& entity = Deref(pEntity);
    gpAddReferenceTo(entity);

    gpNameOf(entity)     = name;
    gpPositionOf(entity) = gpDisplacement(200, 300, 0);
    gpMassOf(entity)     = gpMass(5.0f);

    gpVec3 halfExtends(50.0f, 50.0f, 0.0f);
    gpInverseInertiaOf(entity) = gpInverseInertia::SolidCuboid(gpMassOf(entity), 2.0f * halfExtends);

    gpShapePtrOf(entity) = gpShapeBase::NewBox(halfExtends);

    EZ_VERIFY(gpAddTo(world, entity).Succeeded(), "Failed to add entity to world.");

    gpApplyForceTo(entity, gpForce(20000, 0, 0), gpTime(1), gpDisplacement(0, 1, 0));
}

static gpEntity* Particle(gpWorld& world, const char* name, const gpDisplacement& position)
{
    auto pParticle = gpNew<gpEntity>();
    auto& p = Deref(pParticle);

    gpNameOf(p)     = name;
    gpPositionOf(p) = position;
    gpMassOf(p)     = gpMass(1);

    auto result = gpAddTo(world, p);
    EZ_VERIFY(result.Succeeded(), "Failed to add particle to world.");

    return pParticle;
}

static void Populate(gpWorld& world)
{
    gpGravityOf(world) = gpLinearAcceleration(gpVec3(0, 0.0f, 0));

    //RotatingRigidBody("Player", world);

    auto pLeft  = Particle(world, "left",  gpDisplacement(150, 250, 0));
    auto pRight = Particle(world, "right", gpDisplacement(350, 250, 0));

    gpScalar magnitude = 10.0f;
    gpApplyLinearImpulseTo(Deref(pLeft),  gpLinearVelocity( magnitude, 0, 0));
    gpApplyLinearImpulseTo(Deref(pRight), gpLinearVelocity(-magnitude, 0, 0));
}

static void Cleanup()
{
    gpDelete(g_pWorld);
}

static void OnRenderExtraction(gpRenderExtractor* pExtractor)
{
    auto dt = ezClock::Get()->GetAccumulatedTime().GetSeconds();
    auto angle = ezAngle::Radian(static_cast<float>(-dt));
    angle.NormalizeRange();

    auto pLine = pExtractor->AllocateRenderData<gpDrawData::Line>();
    pLine->m_Start.Set(250, 250, 0);
    pLine->m_End = pLine->m_Start + gpVec3(ezMath::Cos(angle), ezMath::Sin(angle), 0.0f) * 200.0f;
    pLine->m_fLineWidth = angle.GetRadian() * 2;
    pLine->m_Color = ezColor(1.0f, 1.0f, 1.0f, 1.0f);

    auto pArrow = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
    pArrow->m_Start.Set(300, 100, 0.0f);
    pArrow->m_End.Set(250, 150, 0.0f);
    pArrow->m_fLineWidth = 1;
    pArrow->m_Color = ezColor(0.0f, 1.0f, 1.0f, 1.0f);

    auto pPoint = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pPoint->m_Position.Set(400, 100, 0.0f);
    pPoint->m_Color.SetRGB(ezVec3(1.0f, 0.0f, 0.0f));
    pPoint->m_fPointSize = angle.GetRadian() * 2;

    auto pCircle = pExtractor->AllocateRenderData<gpDrawData::Circle>();
    pCircle->m_Position.Set(100, 300, 0);
    pCircle->m_FillColor = ezColor(0.0f, 1.0f, 0.0f, 0.75f);
    pCircle->m_OutlineColor = ezColor(0.0f, 0.0f, 0.0f, 1.0f);
    pCircle->m_fOutlineWidth = angle.GetRadian() * 2;
    pCircle->m_fRadius = 50.0f;
    pCircle->m_uiNumLineSegments = 100;

    auto pPoly = pExtractor->AllocateRenderData<gpDrawData::Polygon>();
    pPoly->m_FillColor = ezColor(1.0f, 0.0f, 0.0f, 0.5f);
    pPoly->m_OutlineColor = ezColor(1.0f, 1.0f, 0.0f, 1.0f);
    static gpVec3 vertices[3] =
    {
        gpVec3(0.5f, 0.5f, 0.0f) * 100.0f,
        gpVec3(2.0f, 1.0f, 0.0f) * 100.0f,
        gpVec3(1.0f, 2.0f, 0.0f) * 100.0f,
    };
    pPoly->m_Vertices = vertices;
}

void gpExperimentsApp::AfterEngineInit()
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
        SetupRendering();
    }

    //gpRenderExtractor::AddExtractionListener(gpRenderExtractionListener(OnRenderExtraction));
    gpRenderExtractor::AddExtractionListener([](gpRenderExtractor* pExtractor){
        gpExtractRenderDataOf(Deref(g_pWorld), pExtractor);
    });

    g_pWorld = gpNew<gpWorld>("World");
    Populate(Deref(g_pWorld));
}

void gpExperimentsApp::BeforeEngineShutdown()
{
    ::Cleanup();

    ezStartup::ShutdownEngine();
    Cleanup();

    ezPlugin::UnloadPlugin("ezInspectorPlugin");
    ezTelemetry::CloseConnection();
}

ezApplication::ApplicationExecution gpExperimentsApp::Run()
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
        gpStepSimulationOf(Deref(g_pWorld), tUpdateInterval);

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

#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

#include <gpCore/Shapes.h>
#include <gpCore/World/Entity.h>
#include <gpCore/Algorithm/CollisionDetection.h>

EZ_CREATE_SIMPLE_TEST_GROUP(Collision);

const auto e = ezMath::BasicType<gpScalar>::DefaultEpsilon();

EZ_CREATE_SIMPLE_TEST(Collision, Particle_Particle)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "")
    {
        auto& left  = Deref(gpNew<gpEntity>());
        auto& right = Deref(gpNew<gpEntity>());

        EZ_TEST_BOOL(gpAreColliding(left, right));

        gpPositionOf(right) = gpDisplacement(1, 0, 0);
        EZ_TEST_BOOL(!gpAreColliding(left, right));
    }
}

EZ_CREATE_SIMPLE_TEST(Collision, Particle_Sphere)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "")
    {
        auto& left  = Deref(gpNew<gpEntity>());
        auto& right = Deref(gpNew<gpEntity>());
        gpShapePtrOf(right) = gpNew<gpSphereShape>(1.1f);

        EZ_TEST_BOOL(gpAreColliding(left, right));

        gpPositionOf(right) = gpDisplacement(1, 0, 0);
        EZ_TEST_BOOL(gpAreColliding(left, right));

        gpPositionOf(right) = gpDisplacement(2, 0, 0);
        EZ_TEST_BOOL(!gpAreColliding(left, right));
    }
}

EZ_CREATE_SIMPLE_TEST(Collision, Sphere_Sphere)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "")
    {
        auto& left  = Deref(gpNew<gpEntity>());
        gpShapePtrOf(left) = gpNew<gpSphereShape>(1.0f);
        auto& right = Deref(gpNew<gpEntity>());
        gpShapePtrOf(right) = gpNew<gpSphereShape>(1.0f);

        EZ_TEST_BOOL(gpAreColliding(left, right));

        gpPositionOf(left)  = gpDisplacement(-1, 0, 0);
        gpPositionOf(right) = gpDisplacement( 1, 0, 0);
        EZ_TEST_BOOL(gpAreColliding(left, right));

        gpPositionOf(left)  = gpDisplacement(-2, 0, 0);
        gpPositionOf(right) = gpDisplacement( 2, 0, 0);
        EZ_TEST_BOOL(!gpAreColliding(left, right));
    }
}

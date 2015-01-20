#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

#include <gpCore/Shapes/Circle.h>
#include <gpCore/World/RigidBody.h>
#include <gpCore/Dynamics/CollisionDetection.h>

EZ_CREATE_SIMPLE_TEST_GROUP(CollisionDetection);

const auto e = ezMath::BasicType<gpScalar>::DefaultEpsilon();

EZ_CREATE_SIMPLE_TEST(CollisionDetection, CircleCircle)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Collision")
    {
        gpRigidBody body1;
        gpRigidBody body2;
    }
}

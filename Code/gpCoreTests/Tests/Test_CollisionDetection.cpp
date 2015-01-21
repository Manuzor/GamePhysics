#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

#include <gpCore/Shapes/Shape.h>
#include <gpCore/World/Entity.h>
#include <gpCore/Dynamics/CollisionDetection.h>

EZ_CREATE_SIMPLE_TEST_GROUP(CollisionDetection);

const auto e = ezMath::BasicType<gpScalar>::DefaultEpsilon();

EZ_CREATE_SIMPLE_TEST(CollisionDetection, Circle_Circle)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Collision")
    {
        gpEntity body1;
        gpEntity body2;
    }
}

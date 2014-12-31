#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

EZ_CREATE_SIMPLE_TEST_GROUP(Units);

#define EPSILON ezMath::BasicType<gpScalar>::DefaultEpsilon()

EZ_CREATE_SIMPLE_TEST(Units, Basics)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Mass")
    {
        auto m = gpMass(42);
        EZ_TEST_FLOAT(gpValueOf(m), 42, EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Displacement")
    {
        auto d = gpDisplacement(gpVec3(1.0f, 2.0f, 3.0f));
        EZ_TEST_VEC3(gpValueOf(d), gpVec3(1.0f, 2.0f, 3.0f), EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Time")
    {
        auto t = gpTime(1337);
        EZ_TEST_FLOAT(gpValueOf(t), 1337, EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Velocity")
    {
        auto v = gpVelocity(gpVec3(1, 2, 3));
        EZ_TEST_VEC3(gpValueOf(v), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_BOOL(IsEqual(v, gpVelocity(gpVec3(1, 2, 3))));

        v = gpVelocity(gpDisplacement(gpVec3(1, 2, 3)), gpTime(0.5));
        EZ_TEST_VEC3(gpValueOf(v), gpVec3(2, 4, 6), EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Acceleration")
    {
        auto a = gpAcceleration(gpVec3(1, 2, 3));
        EZ_TEST_VEC3(gpValueOf(a), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_BOOL(IsEqual(a, gpAcceleration(gpVec3(1, 2, 3))));

        a = gpAcceleration(gpDisplacement(gpVec3(1, 2, 3)), gpTime(ezMath::Sqrt(0.5)));
        EZ_TEST_VEC3(gpValueOf(a), gpVec3(2, 4, 6), EPSILON);
        EZ_TEST_BOOL(IsEqual(a, gpAcceleration(gpDisplacement(gpVec3(1, 2, 3)), gpTime(ezMath::Sqrt(0.5)))));
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Force")
    {
        auto f = gpForce(gpVec3(1, 2, 3));

        EZ_TEST_VEC3(gpValueOf(f), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_BOOL(IsEqual(f, gpForce(gpVec3(1, 2, 3))));

        const auto m = gpMass(42);
        f = gpForce(m, gpAcceleration(gpVec3(1, 2, 3)));
        EZ_TEST_VEC3(gpValueOf(f), gpVec3(1 * gpValueOf(m), 2 * gpValueOf(m), 3 * gpValueOf(m)), EPSILON);
        EZ_TEST_BOOL(IsEqual(f, gpForce(m, gpAcceleration(gpVec3(1, 2, 3)))));
    }
}

EZ_CREATE_SIMPLE_TEST(Units, Algorithms)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "a = F / m")
    {
        auto f = gpForce(gpMass(10), gpAcceleration(gpDisplacement(gpVec3(10, 0, 0)), // 10 meters in x direction
                                                    gpTime(1)));                      // per second
        auto a = f / gpMass(5);
        EZ_TEST_BOOL(IsEqual(a, gpAcceleration(gpVec3(20, 0, 0)))); // 20 meters in x direction per second
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "v = a * t")
    {
        auto a = gpAcceleration(gpDisplacement(gpVec3(1, 2, 3)), gpTime(ezMath::Sqrt(10.0)));
        auto v = a * gpTime(5);
        auto test = gpVelocity(gpDisplacement(gpVec3(1, 2, 3)), gpTime(2));
        EZ_TEST_BOOL(IsEqual(v, test));
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "p = v * t")
    {
        auto v = gpVelocity(gpVec3(1, 2, 3));
        auto t = gpTime(2);
        auto p = v * t;
        EZ_TEST_VEC3(gpValueOf(p), gpVec3(2, 4, 6), EPSILON);
    }
}

#undef EPSILON

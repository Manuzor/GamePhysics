#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

EZ_CREATE_SIMPLE_TEST_GROUP(Units);

const auto e = ezMath::BasicType<gpScalar>::DefaultEpsilon();

EZ_CREATE_SIMPLE_TEST(Units, Basics)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Mass")
    {
        auto m = gpMass(42);
        EZ_TEST_FLOAT(gpValueOf(m), 42, e);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Displacement")
    {
        auto d = gpDisplacement(1.0f, 2.0f, 3.0f);
        EZ_TEST_VEC3(gpValueOf(d), gpVec3(1.0f, 2.0f, 3.0f), e);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Time")
    {
        auto t = gpTime(1337);
        EZ_TEST_FLOAT(gpValueOf(t), 1337, e);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Velocity")
    {
        auto v = gpLinearVelocity(1, 2, 3);
        EZ_TEST_VEC3(gpValueOf(v), gpVec3(1, 2, 3), e);
        EZ_TEST_BOOL(gpAreEqual(v, gpLinearVelocity(1, 2, 3)));

        v = gpDisplacement(1, 2, 3) / gpTime(0.5);
        EZ_TEST_VEC3(gpValueOf(v), gpVec3(2, 4, 6), e);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Acceleration")
    {
        auto a = gpLinearAcceleration(1, 2, 3);
        EZ_TEST_VEC3(gpValueOf(a), gpVec3(1, 2, 3), e);
        EZ_TEST_BOOL(gpAreEqual(a, gpLinearAcceleration(1, 2, 3)));

        const auto t = gpTime(ezMath::Sqrt(0.5));
        a = (gpDisplacement(1, 2, 3) / t) / t;
        EZ_TEST_VEC3(gpValueOf(a), gpVec3(2, 4, 6), e);
        EZ_TEST_BOOL(gpAreEqual(a, (gpDisplacement(1, 2, 3) / t) / t));
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Force")
    {
        gpForce f(1, 2, 3);

        EZ_TEST_VEC3(gpValueOf(f), gpVec3(1, 2, 3), e);
        EZ_TEST_BOOL(gpAreEqual(f, gpForce(gpVec3(1, 2, 3))));

        const gpMass m(42);
        f = m * gpLinearAcceleration(1, 2, 3);
        EZ_TEST_VEC3(gpValueOf(f), gpVec3(1 * gpValueOf(m), 2 * gpValueOf(m), 3 * gpValueOf(m)), e);
        EZ_TEST_BOOL(gpAreEqual(f, m * gpLinearAcceleration(1, 2, 3)));
    }
}

EZ_CREATE_SIMPLE_TEST(Units, Algorithms)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "a = F / m")
    {
        gpForce f = gpMass(10) * gpLinearAcceleration(10, 0, 0); // 10 meters in x direction per second per second
        auto a = f / gpMass(5);
        EZ_TEST_BOOL(gpAreEqual(a, gpLinearAcceleration(20, 0, 0))); // 20 meters in x direction per second per second
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "v = a * t")
    {
        auto t = gpTime(ezMath::Sqrt(10.0));
        auto a = (gpDisplacement(1, 2, 3) / t) / t;
        t = gpTime(5);
        auto v = a * t;
        auto test = gpDisplacement(1, 2, 3) / gpTime(2);
        EZ_TEST_BOOL(gpAreEqual(v, test));
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "p = v * t")
    {
        auto v = gpLinearVelocity(gpVec3(1, 2, 3));
        auto t = gpTime(2);
        auto p = v * t;
        EZ_TEST_VEC3(gpValueOf(p), gpVec3(2, 4, 6), e);
    }
}

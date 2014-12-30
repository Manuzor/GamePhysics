#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

EZ_CREATE_SIMPLE_TEST_GROUP(Units);

#define EPSILON ezMath::BasicType<gpScalar>::DefaultEpsilon()

EZ_CREATE_SIMPLE_TEST(Units, Basics)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Mass")
    {
        auto m = Mass(42);
        EZ_TEST_FLOAT(ValueOf(m), 42, EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Displacement")
    {
        auto d = Displacement(gpVec3(1.0f, 2.0f, 3.0f));
        EZ_TEST_VEC3(ValueOf(d), gpVec3(1.0f, 2.0f, 3.0f), EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Time")
    {
        auto t = Time(1337);
        EZ_TEST_FLOAT(ValueOf(t), 1337, EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Velocity")
    {
        auto v = Velocity(gpVec3(1, 2, 3));
        EZ_TEST_VEC3(ValueOf(v), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_BOOL(IsEqual(v, Velocity(gpVec3(1, 2, 3))));

        v = Velocity(Displacement(gpVec3(1, 2, 3)), Time(0.5));
        EZ_TEST_VEC3(ValueOf(v), gpVec3(2, 4, 6), EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Acceleration")
    {
        auto a = Acceleration(gpVec3(1, 2, 3));
        EZ_TEST_VEC3(ValueOf(a), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_BOOL(IsEqual(a, Acceleration(gpVec3(1, 2, 3))));

        a = Acceleration(Displacement(gpVec3(1, 2, 3)), Time(ezMath::Sqrt(0.5)));
        EZ_TEST_VEC3(ValueOf(a), gpVec3(2, 4, 6), EPSILON);
        EZ_TEST_BOOL(IsEqual(a, Acceleration(Displacement(gpVec3(1, 2, 3)), Time(ezMath::Sqrt(0.5)))));
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Force")
    {
        auto f = Force(gpVec3(1, 2, 3));

        EZ_TEST_VEC3(ValueOf(f), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_BOOL(IsEqual(f, Force(gpVec3(1, 2, 3))));

        const auto m = Mass(42);
        f = Force(m, Acceleration(gpVec3(1, 2, 3)));
        EZ_TEST_VEC3(ValueOf(f), gpVec3(1 * ValueOf(m), 2 * ValueOf(m), 3 * ValueOf(m)), EPSILON);
        EZ_TEST_BOOL(IsEqual(f, Force(m, Acceleration(gpVec3(1, 2, 3)))));
    }
}

EZ_CREATE_SIMPLE_TEST(Units, Algorithms)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "a = F / m")
    {
        auto f = Force(Mass(10), Acceleration(Displacement(gpVec3(10, 0, 0)), // 10 meters in x direction
                                              Time(1)));                      // per second
        auto a = f / Mass(5);
        EZ_TEST_BOOL(IsEqual(a, Acceleration(Displacement(gpVec3(20, 0, 0)), // 20 meters in x direction
                                             Time(1))));                     // per second
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "v = a * t")
    {
        auto a = Acceleration(Displacement(gpVec3(1, 2, 3)), Time(ezMath::Sqrt(10.0)));
        auto v = a * Time(5);
        auto test = Velocity(Displacement(gpVec3(1, 2, 3)),
                             Time(2));
        EZ_TEST_BOOL(IsEqual(v, test));
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "p = v * t")
    {
        auto v = Velocity(gpVec3(1, 2, 3));
        auto t = Time(2);
        auto p = v * t;
        EZ_TEST_VEC3(ValueOf(p), gpVec3(2, 4, 6), EPSILON);
    }
}

#undef EPSILON

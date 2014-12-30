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
        auto v = Velocity(Displacement(gpVec3(1, 2, 3)), Time(4));
        EZ_TEST_VEC3(ValueOf(DisplacementOf(v)), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_FLOAT(ValueOf(TimeOf(v)), 4, EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Acceleration")
    {
        auto a = Acceleration(Displacement(gpVec3(1, 2, 3)), Time(4));
        EZ_TEST_VEC3(ValueOf(DisplacementOf(a)), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_FLOAT(ValueOf(TimeOf(a)), 4, EPSILON);
    }

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Force")
    {
        auto f = Force(Mass(42), Acceleration(Displacement(gpVec3(1, 2, 3)), Time(1337)));

        EZ_TEST_FLOAT(ValueOf(MassOf(f)), 42, EPSILON);
        EZ_TEST_VEC3(ValueOf(DisplacementOf(AccelerationOf(f))), gpVec3(1, 2, 3), EPSILON);
        EZ_TEST_FLOAT(ValueOf(TimeOf(AccelerationOf(f))), 1337, EPSILON);
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

    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Integrate")
    {

    }
}

#undef EPSILON

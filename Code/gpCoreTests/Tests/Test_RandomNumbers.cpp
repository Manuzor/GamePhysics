#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

#include "gpCore/Utilities/RandomNumbers.h"

EZ_CREATE_SIMPLE_TEST_GROUP(Random);

EZ_CREATE_SIMPLE_TEST(Random, Basics)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Integer")
    {
        gpRandomNumberGenerator Rand;
        Rand.SetSeed(0);

        ezInt32 FirstSet[] = {
            Rand.GenerateInteger(0, 10),
            Rand.GenerateInteger(0, 10),
            Rand.GenerateInteger(0, 10),
        };

        Rand.Reset();

        ezInt32 SecondSet[] = {
            Rand.GenerateInteger(0, 10),
            Rand.GenerateInteger(0, 10),
            Rand.GenerateInteger(0, 10),
        };

        for (size_t i = 0; i < EZ_ARRAY_SIZE(FirstSet); ++i)
        {
            EZ_TEST_INT(FirstSet[i], SecondSet[i]);
        }
    }
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Float")
    {
        gpRandomNumberGenerator Rand;

        float FirstSet[] = {
            Rand.GenerateFloat(0.0f, 10.0f),
            Rand.GenerateFloat(0.0f, 10.0f),
            Rand.GenerateFloat(0.0f, 10.0f),
        };

        Rand.Reset();

        float SecondSet[] = {
            Rand.GenerateFloat(0.0f, 10.0f),
            Rand.GenerateFloat(0.0f, 10.0f),
            Rand.GenerateFloat(0.0f, 10.0f),
        };

        for (size_t i = 0; i < EZ_ARRAY_SIZE(FirstSet); ++i)
        {
            EZ_TEST_FLOAT(FirstSet[i], SecondSet[i], 0);
        }
    }
}

EZ_CREATE_SIMPLE_TEST(Random, Randomize)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "Vec3")
    {
        using ezVec3T = gpVec3;

        gpRandomNumberGenerator Rand;
        gpVec3 ToRandomize(0.0f);
        gpVec3 ToCompare(1.0f);

        gpRandomize(&Rand, ToRandomize);
        Rand.Reset();
        gpRandomize(&Rand, ToCompare);

        EZ_TEST_VEC3(ToRandomize, ToCompare, 0.0001f);
    }
}

#include "gpCoreTests/PCH.h"
#include <TestFramework/Framework/TestFramework.h>

EZ_CREATE_SIMPLE_TEST_GROUP(Task);

EZ_CREATE_SIMPLE_TEST(Task, Basics)
{
    EZ_TEST_BLOCK(ezTestBlock::Enabled, "First Try")
    {
        EZ_TEST_BOOL(true);
    }
}

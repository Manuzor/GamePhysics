#include "gpCore/PCH.h"
#include "gpCore/Utilities/RandomNumbers.h"

static std::random_device g_RandomDevice;

void gpRandomNumberGenerator::RandomizeSeed()
{
    SetSeed(g_RandomDevice());
}

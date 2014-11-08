#pragma once

#include <random>

class GP_CoreAPI gpRandomNumberGenerator
{
public:
    gpRandomNumberGenerator() { RandomizeSeed(); }

    void RandomizeSeed();
    void SetSeed(ezUInt32 uiNewSeed) { m_uiSeed = uiNewSeed; m_Engine.seed(uiNewSeed); }
    ezUInt32 GetSeed() const { return m_uiSeed; }
    void Reset() { SetSeed(GetSeed()); }

    template<typename IntegerType>
    IntegerType GenerateInteger(IntegerType iMin = 0, IntegerType iMax = std::numeric_limits<IntegerType>::max())
    {
        std::uniform_int_distribution<IntegerType> Generator(iMin, iMax);
        return Generator(m_Engine);
    }

    template<typename FloatType>
    FloatType GenerateFloat(FloatType fMin = 0.0f, FloatType fMax = 1.0f)
    {
        std::uniform_real_distribution<FloatType> Generator(fMin, fMax);
        return Generator(m_Engine);
    }

private:
    ezUInt32 m_uiSeed;
    std::default_random_engine m_Engine;
};

void gpRandomize(gpRandomNumberGenerator* pRand, gpVec3& Vector, gpScalar fMin = 0.0f, gpScalar fMax = 1.0f)
{
    Vector.x = pRand->GenerateFloat(fMin, fMax);
    Vector.y = pRand->GenerateFloat(fMin, fMax);
    Vector.z = pRand->GenerateFloat(fMin, fMax);
}

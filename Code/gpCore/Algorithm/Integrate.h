#pragma once

EZ_FORCE_INLINE gpVec3 gpIntegrate(const gpVec3& vector, ezTime dt)
{
    return vector * (float)dt.GetSeconds();
}

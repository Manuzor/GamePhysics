
EZ_FORCE_INLINE bool gpContains(const gpPhysicalProperties& props, const gpCircleShape& Circle, const gpVec3& Point)
{
    auto fRadiusSquared = gpRadiusOf(Circle) * gpRadiusOf(Circle);
    auto Diff = Point - gpPositionOf(props);
    return Diff.GetLengthSquared() <= fRadiusSquared;
}


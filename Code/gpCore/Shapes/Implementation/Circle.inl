
EZ_FORCE_INLINE bool gpContains(const gpPhysicalProperties& props,
                                const gpCircleShape& Circle,
                                const gpDisplacement& Point)
{
    auto fRadiusSquared = gpRadiusOf(Circle) * gpRadiusOf(Circle);
    auto Diff = Point - gpPositionOf(props);
    return gpValueOf(Diff).GetLengthSquared() <= fRadiusSquared;
}


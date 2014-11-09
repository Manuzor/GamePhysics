
EZ_FORCE_INLINE bool gpContains(const gpPhysicalProperties* pProps, const gpCircleShape& Circle, const gpVec3& Point)
{
    auto fRadiusSquared = Circle.GetRadius() * Circle.GetRadius();
    auto Diff = Point - pProps->m_Position;
    return Diff.GetLengthSquared() <= fRadiusSquared;
}


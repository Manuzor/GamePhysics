
EZ_FORCE_INLINE bool gpContains(const gpRectF& Rectangle, const gpVec3& Point)
{
    bool bHorizontal;
    if (ezMath::Sign(Rectangle.width) > 0.0f)
        bHorizontal = Point.x >= Rectangle.x && Point.x <= Rectangle.x + Rectangle.width;
    else
        bHorizontal = Point.x <= Rectangle.x && Point.x >= Rectangle.x + Rectangle.width;

    bool bVertical;
    if (ezMath::Sign(Rectangle.height) > 0.0f)
        bVertical = Point.y >= Rectangle.y && Point.y <= Rectangle.y + Rectangle.height;
    else
        bVertical = Point.y <= Rectangle.y && Point.y >= Rectangle.y + Rectangle.height;

    return bHorizontal && bVertical;
}


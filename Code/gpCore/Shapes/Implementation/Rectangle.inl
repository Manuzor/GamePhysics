
EZ_FORCE_INLINE bool gpContains(const gpRectF& Rectangle, const gpDisplacementUnit& Point)
{
    const auto& Pos = gpValueOf(Point);

    bool bHorizontal;
    if (ezMath::Sign(Rectangle.width) > 0.0f)
        bHorizontal = Pos.x >= Rectangle.x && Pos.x <= Rectangle.x + Rectangle.width;
    else
        bHorizontal = Pos.x <= Rectangle.x && Pos.x >= Rectangle.x + Rectangle.width;

    bool bVertical;
    if (ezMath::Sign(Rectangle.height) > 0.0f)
        bVertical = Pos.y >= Rectangle.y && Pos.y <= Rectangle.y + Rectangle.height;
    else
        bVertical = Pos.y <= Rectangle.y && Pos.y >= Rectangle.y + Rectangle.height;

    return bHorizontal && bVertical;
}


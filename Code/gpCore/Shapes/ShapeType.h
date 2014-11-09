#pragma once

//EZ_DECLARE_FLAGS(ezUInt8, gpShapeFlags, Convex);

namespace gpInternal
{
    struct gpShapeType
    {
        enum Enum
        {
            DefaultInit = -1,

            Circle,
            Line,
            Polygon,

            ENUM_COUNT
        };
    };
}

using gpShapeType = ezEnum<gpInternal::gpShapeType, ezUInt8>;

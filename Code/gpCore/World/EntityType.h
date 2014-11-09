#pragma once

namespace gpInternal
{
    struct gpEntityTypeBase
    {
        enum Enum
        {
            Particle,
            ForceField,
            RigidBody,
        };

        EZ_ENUM_TO_STRING(Particle, ForceField, RigidBody);
    };
}

using gpEntityType = ezEnum<gpInternal::gpEntityTypeBase, ezUInt8>;

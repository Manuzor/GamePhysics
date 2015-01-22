#pragma once

class gpEntity;

GP_CoreAPI void gpResolveCollision(gpEntity& lhs, gpEntity& rhs);

//////////////////////////////////////////////////////////////////////////

class gpPhysicalProperties;
class gpShape;

GP_CoreAPI void gpResolveCollision(gpPhysicalProperties& lhsProps, const gpShape& lhsShape,
                                   gpPhysicalProperties& rhsProps, const gpShape& rhsShape);


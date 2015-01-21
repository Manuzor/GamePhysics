#pragma once

class gpEntity;

GP_CoreAPI void gpResolveCollision(const gpEntity& lhs, const gpEntity& rhs);

//////////////////////////////////////////////////////////////////////////

class gpPhysicalProperties;
class gpShape;

GP_CoreAPI void gpResolveCollision(const gpPhysicalProperties& lhsProps, const gpShape& lhsShape,
                                   const gpPhysicalProperties& rhsProps, const gpShape& rhsShape);


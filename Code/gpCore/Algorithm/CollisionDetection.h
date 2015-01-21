#pragma once

class gpEntity;

GP_CoreAPI bool gpAreColliding(const gpEntity& lhs, const gpEntity& rhs);

//////////////////////////////////////////////////////////////////////////

class gpShape;

GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpShape& lhsShape,
                               const gpTransform& rhsTransform, const gpShape& rhsShape);


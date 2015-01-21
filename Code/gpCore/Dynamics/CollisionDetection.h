#pragma once

class gpEntity;

GP_CoreAPI bool gpAreColliding(const gpEntity& lhs, const gpEntity& rhs);

//////////////////////////////////////////////////////////////////////////

class gpShapeBase;
class gpCircleShape;
class gpPolygonShape;

GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpShapeBase& lhsShape,
                               const gpTransform& rhsTransform, const gpShapeBase& rhsShape);


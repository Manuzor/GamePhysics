#pragma once

class gpParticleEntity;
class gpForceFieldEntity;
class gpRigidBody;

GP_CoreAPI bool gpAreColliding(const gpRigidBody& lhs, const gpRigidBody& rhs);

//////////////////////////////////////////////////////////////////////////

class gpShapeBase;
class gpCircleShape;
class gpPolygonShape;

GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpShapeBase& lhsShape,
                               const gpTransform& rhsTransform, const gpShapeBase& rhsShape);

//////////////////////////////////////////////////////////////////////////

GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpCircleShape& lhsShape,
                               const gpTransform& rhsTransform, const gpCircleShape& rhsShape);
GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpCircleShape& lhsShape,
                               const gpTransform& rhsTransform, const gpPolygonShape& rhsShape);
GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpCircleShape& lhsShape,
                               const gpTransform& rhsTransform, const gpShapeBase& rhsShape);

GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpPolygonShape& lhsShape,
                               const gpTransform& rhsTransform, const gpPolygonShape& rhsShape);
GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpPolygonShape& lhsShape,
                               const gpTransform& rhsTransform, const gpCircleShape& rhsShape);
GP_CoreAPI bool gpAreColliding(const gpTransform& lhsTransform, const gpPolygonShape& lhsShape,
                               const gpTransform& rhsTransform, const gpShapeBase& rhsShape);


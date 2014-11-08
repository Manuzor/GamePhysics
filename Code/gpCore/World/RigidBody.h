#pragma once
#include "gpCore/World/EntityBase.h"

class gpShapeBase;

class GP_CoreAPI gpRigidBody : public gpEntityBase
{
    friend gpWorld;
public:
    gpRigidBody();

    gpShapeBase* GetShape() { return m_pShape; }
    const gpShapeBase* GetShape() const { return m_pShape; }
    void SetShape(gpShapeBase* pShape) { m_pShape = pShape; }

private:
    gpShapeBase* m_pShape;
};

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpRigidBody& RigidBody);

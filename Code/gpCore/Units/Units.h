#include "gpCore/Units/UnitBase.h"

// Scalar units
//////////////////////////////////////////////////////////////////////////
#include "gpCore/Utilities/ScalarSyncer.h"

class gpMass : public gpScalarUnitBase<gpMass>
{
public:
    using BaseType = gpScalarUnitBase<gpMass>;
    GP_DefineScalarUnitConstructors(gpMass);
};
using gpMassSyncer = gpScalarSyncerTemplate<gpMass>;

class gpTime : public gpScalarUnitBase<gpTime, double>
{
public:
    using BaseType = gpScalarUnitBase<gpTime, double>;
    GP_DefineScalarUnitConstructors(gpTime);
    gpTime(ezTime t) : BaseType(t.GetSeconds()) {}
};

// Angular kinematics, Mat3 units
//////////////////////////////////////////////////////////////////////////

class gpOrientation : public gpMat3UnitBase<gpOrientation>
{
public:
    using BaseType = gpMat3UnitBase<gpOrientation>;
    GP_DefineMat3UnitConstructors(gpOrientation);
    gpOrientation(const gpVec3& v) : BaseType(gpZero)
    {
        gpValueOf(Deref(this)).Element(0, 0) = v.x;
        gpValueOf(Deref(this)).Element(1, 1) = v.y;
        gpValueOf(Deref(this)).Element(2, 2) = v.z;
    }
};

class gpInverseInertia : public gpMat3UnitBase<gpInverseInertia>
{
public:
    using BaseType = gpMat3UnitBase<gpInverseInertia>;
    GP_DefineMat3UnitConstructors(gpInverseInertia);
};

// Angular kinematics, Vec3 units
//////////////////////////////////////////////////////////////////////////

class gpAngularVelocity : public gpVec3UnitBase<gpAngularVelocity>
{
public:
    using BaseType = gpVec3UnitBase<gpAngularVelocity>;
    GP_DefineVec3UnitConstructors(gpAngularVelocity);
};

class gpAngularAcceleration : public gpVec3UnitBase<gpAngularAcceleration>
{
public:
    using BaseType = gpVec3UnitBase<gpAngularAcceleration>;
    GP_DefineVec3UnitConstructors(gpAngularAcceleration);
};

class gpAngularMomentum : public gpVec3UnitBase<gpAngularMomentum>
{
public:
    using BaseType = gpVec3UnitBase<gpAngularMomentum>;
    GP_DefineVec3UnitConstructors(gpAngularMomentum);
};

class gpTorque : public gpVec3UnitBase<gpTorque>
{
public:
    using BaseType = gpVec3UnitBase<gpTorque>;
    GP_DefineVec3UnitConstructors(gpTorque);
};

// Linear kinematics, Vec3 units
//////////////////////////////////////////////////////////////////////////

class gpDisplacement : public gpVec3UnitBase<gpDisplacement>
{
public:
    using BaseType = gpVec3UnitBase<gpDisplacement>;
    GP_DefineVec3UnitConstructors(gpDisplacement);
};

class gpLinearVelocity : public gpVec3UnitBase<gpLinearVelocity>
{
public:
    using BaseType = gpVec3UnitBase<gpLinearVelocity>;
    GP_DefineVec3UnitConstructors(gpLinearVelocity);
};

class gpLinearAcceleration : public gpVec3UnitBase<gpLinearAcceleration>
{
public:
    using BaseType = gpVec3UnitBase<gpLinearAcceleration>;
    GP_DefineVec3UnitConstructors(gpLinearAcceleration);
};

class gpForce : public gpVec3UnitBase<gpForce>
{
public:
    using BaseType = gpVec3UnitBase<gpForce>;
    GP_DefineVec3UnitConstructors(gpForce);
};

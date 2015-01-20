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

// Angular kinematics, Scalar units
//////////////////////////////////////////////////////////////////////////

class gpOrientation : public gpScalarUnitBase<gpOrientation, gpScalar>
{
public:
    using BaseType = gpScalarUnitBase<gpOrientation, gpScalar>;
    GP_DefineScalarUnitConstructors(gpOrientation);
    gpOrientation(ezAngle a) : BaseType(a.GetRadian()) {}

    EZ_FORCE_INLINE static gpOrientation Radians(gpScalar amount) { return gpOrientation(amount); }
    EZ_FORCE_INLINE static gpOrientation Degrees(gpScalar amount) { return gpOrientation(ezAngle::DegToRad(amount)); }
};
using gpOrientationSyncer = gpScalarSyncerTemplate<gpOrientation>;

class gpAngularVelocity : public gpScalarUnitBase<gpAngularVelocity, gpScalar>
{
public:
    using BaseType = gpScalarUnitBase<gpAngularVelocity, gpScalar>;
    GP_DefineScalarUnitConstructors(gpAngularVelocity);
};

class gpAngularAcceleration : public gpScalarUnitBase<gpAngularAcceleration, gpScalar>
{
public:
    using BaseType = gpScalarUnitBase<gpAngularAcceleration, gpScalar>;
    GP_DefineScalarUnitConstructors(gpAngularAcceleration);
};

class gpInverseInertia : public gpScalarUnitBase<gpInverseInertia, gpScalar>
{
public:
    using BaseType = gpScalarUnitBase<gpInverseInertia, gpScalar>;
    GP_DefineScalarUnitConstructors(gpInverseInertia);
};

class gpTorque : public gpScalarUnitBase<gpTorque, gpScalar>
{
public:
    using BaseType = gpScalarUnitBase<gpTorque, gpScalar>;
    GP_DefineScalarUnitConstructors(gpTorque);
};

class gpAngularMomentum : public gpScalarUnitBase<gpAngularMomentum, gpScalar>
{
public:
    using BaseType = gpScalarUnitBase<gpAngularMomentum, gpScalar>;
    GP_DefineScalarUnitConstructors(gpAngularMomentum);
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

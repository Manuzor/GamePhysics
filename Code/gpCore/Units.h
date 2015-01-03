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

// Vec3 units
//////////////////////////////////////////////////////////////////////////

class gpDisplacement : public gpVec3UnitBase<gpDisplacement>
{
public:
    using BaseType = gpVec3UnitBase<gpDisplacement>;
    GP_DefineVec3UnitConstructors(gpDisplacement);
};

class gpVelocity : public gpVec3UnitBase<gpVelocity>
{
public:
    using BaseType = gpVec3UnitBase<gpVelocity>;
    GP_DefineVec3UnitConstructors(gpVelocity);
};

class gpAcceleration : public gpVec3UnitBase<gpAcceleration>
{
public:
    using BaseType = gpVec3UnitBase<gpAcceleration>;
    GP_DefineVec3UnitConstructors(gpAcceleration);
};

class gpForce : public gpVec3UnitBase<gpForce>
{
public:
    using BaseType = gpVec3UnitBase<gpForce>;
    GP_DefineVec3UnitConstructors(gpForce);
};

//GP_DefineVec3Unit(gpDisplacement, gpDisplacement)
//GP_DefineVec3Unit(gpVelocity,     gpVelocity)
//GP_DefineVec3Unit(gpAcceleration, gpAcceleration)
//GP_DefineVec3Unit(gpForce,        gpForce)

//#include "gpCore/Units/Time.h"

// Algorithms
//////////////////////////////////////////////////////////////////////////
#include "gpCore/Units/Algorithms.h"

#pragma once

template<typename TheDerivedType, typename TheScalarType = gpScalar>
class gpScalarUnitBase
{
public:
    using DerivedType = TheDerivedType;
    using ScalarType  = TheScalarType;

    ScalarType m_Value;

    EZ_FORCE_INLINE gpScalarUnitBase() {}
    EZ_FORCE_INLINE explicit gpScalarUnitBase(ScalarType value) : m_Value(value) {}
    EZ_FORCE_INLINE explicit gpScalarUnitBase(gpZeroInitialization) : m_Value((ScalarType)0) {}
    EZ_FORCE_INLINE explicit gpScalarUnitBase(gpNoInitialization) {}
    EZ_FORCE_INLINE gpScalarUnitBase(const DerivedType& other) : m_Value(gpValueOf(other)) {}

    EZ_FORCE_INLINE friend ScalarType& gpValueOf(      DerivedType& d) { return gpValueOf(d.m_Value); }
    EZ_FORCE_INLINE friend ScalarType  gpValueOf(const DerivedType& d) { return gpValueOf(d.m_Value); }

    EZ_FORCE_INLINE friend bool gpAreEqual(const DerivedType& lhs,
                                           const DerivedType& rhs,
                                           ScalarType epsilon = ezMath::BasicType<ScalarType>::DefaultEpsilon())
    {
        return gpAreEqual(gpValueOf(lhs), gpValueOf(rhs), epsilon);
    }

    EZ_FORCE_INLINE friend void gpInvert(DerivedType& d)
    {
        d = DerivedType(gpInvert(gpValueOf(d)));
    }

    EZ_FORCE_INLINE friend DerivedType gpInverseOf(const DerivedType& d)
    {
        return DerivedType(gpInverseOf(gpValueOf(d)));
    }

    EZ_FORCE_INLINE friend bool gpIsZero(const DerivedType& d, ScalarType epsilon = ezMath::BasicType<ScalarType>::DefaultEpsilon())
    {
        return gpIsZero(gpValueOf(d), epsilon);
    }

    EZ_FORCE_INLINE void operator =(const DerivedType& other) { gpValueOf(Deref(this)) = gpValueOf(other); }

    EZ_FORCE_INLINE friend DerivedType operator +(const DerivedType& d1, const DerivedType& d2) { return DerivedType(gpValueOf(d1) + gpValueOf(d2)); }
    EZ_FORCE_INLINE friend DerivedType operator -(const DerivedType& d1, const DerivedType& d2) { return DerivedType(gpValueOf(d1) - gpValueOf(d2)); }
    EZ_FORCE_INLINE friend DerivedType operator *(const DerivedType& d, ScalarType factor)      { return DerivedType(gpValueOf(d)  * factor); }
    EZ_FORCE_INLINE friend DerivedType operator *(ScalarType factor, const DerivedType& d)      { return d * factor; }
    EZ_FORCE_INLINE friend DerivedType operator /(const DerivedType& d, ScalarType factor)      { return d * ezMath::Invert(factor); }
};

#define GP_DefineScalarUnitConstructors(TheUnitName)                                  \
EZ_FORCE_INLINE TheUnitName() {}                                                      \
EZ_FORCE_INLINE explicit TheUnitName(BaseType::ScalarType value) : BaseType(value) {} \
EZ_FORCE_INLINE explicit TheUnitName(gpZeroInitialization z) : BaseType(z) {}         \
EZ_FORCE_INLINE explicit TheUnitName(gpNoInitialization n) : BaseType(n) {}           \
EZ_FORCE_INLINE TheUnitName(const TheUnitName& other) : BaseType(other) {}

// 3-Dimensional Vector
//////////////////////////////////////////////////////////////////////////

template<typename TheDerivedType, typename TheVec3Type = gpVec3>
class gpVec3UnitBase
{
public:
    using DerivedType = TheDerivedType;
    using Vec3Type    = TheVec3Type;
    using ScalarType  = typename Vec3Type::ComponentType;

    Vec3Type m_Value;

    EZ_FORCE_INLINE gpVec3UnitBase() {}
    EZ_FORCE_INLINE explicit gpVec3UnitBase(const Vec3Type& value) : m_Value(value) {}
    EZ_FORCE_INLINE explicit gpVec3UnitBase(ScalarType x, ScalarType y, ScalarType z) : m_Value(x, y, z) {}
    EZ_FORCE_INLINE explicit gpVec3UnitBase(ScalarType xyz) : m_Value(xyz) {}
    EZ_FORCE_INLINE explicit gpVec3UnitBase(gpZeroInitialization) : m_Value(Vec3Type::ZeroVector()) {}
    EZ_FORCE_INLINE explicit gpVec3UnitBase(gpNoInitialization) {}
    EZ_FORCE_INLINE gpVec3UnitBase(const DerivedType& other) : m_Value(gpValueOf(other)) {}

    EZ_FORCE_INLINE friend       Vec3Type& gpValueOf(      DerivedType& d) { return gpValueOf(d.m_Value); }
    EZ_FORCE_INLINE friend const Vec3Type& gpValueOf(const DerivedType& d) { return gpValueOf(d.m_Value); }

    EZ_FORCE_INLINE friend void gpSet(DerivedType& d, ScalarType x, ScalarType y, ScalarType z) { gpValueOf(d).Set(x, y, z); }
    EZ_FORCE_INLINE friend void gpSet(DerivedType& d, ScalarType xyz) { gpValueOf(d).Set(xyz); }
    EZ_FORCE_INLINE friend void gpSetZero(DerivedType& d) { gpValueOf(d).SetZero(); }
    EZ_FORCE_INLINE friend ScalarType& gpX(      DerivedType& d) { return gpValueOf(d).x; }
    EZ_FORCE_INLINE friend ScalarType  gpX(const DerivedType& d) { return gpValueOf(d).x; }
    EZ_FORCE_INLINE friend ScalarType& gpY(      DerivedType& d) { return gpValueOf(d).y; }
    EZ_FORCE_INLINE friend ScalarType  gpY(const DerivedType& d) { return gpValueOf(d).y; }
    EZ_FORCE_INLINE friend ScalarType  gpZ(const DerivedType& d) { return gpValueOf(d).z; }
    EZ_FORCE_INLINE friend ScalarType& gpZ(      DerivedType& d) { return gpValueOf(d).z; }
    EZ_FORCE_INLINE friend ScalarType gpLengthOf(DerivedType& d) { return gpValueOf(d).GetLength(); }
    EZ_FORCE_INLINE friend ScalarType gpSquaredLengthOf(DerivedType& d) { return gpValueOf(d).GetLengthSquared(); }
    EZ_FORCE_INLINE friend ScalarType* gpDataOf(DerivedType& d) { return gpValueOf(d).GetData(); }
    EZ_FORCE_INLINE friend ScalarType gpLengthOfAndNormalize(DerivedType& d) { return gpValueOf(d).GetLengthAndNormalize(); }
    EZ_FORCE_INLINE friend DerivedType gpNormalized(const DerivedType& d) { return gpValueOf(d).GetNormalized(); }
    EZ_FORCE_INLINE friend void gpNormalize(DerivedType& d) { return gpValueOf(d).Normalize(); }
    EZ_FORCE_INLINE friend ezResult gpNormalizeIfNotZero(DerivedType& d,
                                                         const DerivedType& fallback,
                                                         ScalarType epsilon = ezMath::BasicType<ScalarType>::SmallEpsilon())
    {
        return gpValueOf(d).NormalizeIfNotZero(gpValueOf(fallback), epsilon);
    }

    EZ_FORCE_INLINE friend bool gpIsZero(const DerivedType& d)
    {
        return gpIsZero(gpValueOf(d));
    }

    EZ_FORCE_INLINE friend bool gpAreEqual(const DerivedType& lhs,
                                    const DerivedType& rhs,
                                    ScalarType epsilon = ezMath::BasicType<ScalarType>::DefaultEpsilon())
    {
        return gpAreEqual(gpValueOf(lhs), gpValueOf(rhs), epsilon);
    }

    EZ_FORCE_INLINE friend DerivedType operator +(const DerivedType& d1, const DerivedType& d2) { return DerivedType(gpValueOf(d1) + gpValueOf(d2)); }
    EZ_FORCE_INLINE friend DerivedType operator -(const DerivedType& d1, const DerivedType& d2) { return DerivedType(gpValueOf(d1) - gpValueOf(d2)); }
    EZ_FORCE_INLINE friend DerivedType operator *(const DerivedType& d, ScalarType factor)      { return DerivedType(gpValueOf(d)  * factor); }
    EZ_FORCE_INLINE friend DerivedType operator *(ScalarType factor, const DerivedType& d)      { return d * factor; }
    EZ_FORCE_INLINE friend DerivedType operator /(const DerivedType& d, ScalarType factor)      { return d * ezMath::Invert(factor); }

    EZ_FORCE_INLINE void operator = (const DerivedType& other) { gpValueOf(Self()) = gpValueOf(other); }
    EZ_FORCE_INLINE void operator +=(const DerivedType& other) { Self() = Self() + other; }
    EZ_FORCE_INLINE void operator -=(const DerivedType& other) { Self() = Self() - other; }
    EZ_FORCE_INLINE void operator *=(ScalarType factor)        { Self() = Self() * factor; }
    EZ_FORCE_INLINE void operator /=(ScalarType factor)        { Self() = Self() / factor; }

private:
    EZ_FORCE_INLINE       DerivedType& Self()       { return static_cast<DerivedType&>(Deref(this)); }
    EZ_FORCE_INLINE const DerivedType& Self() const { return static_cast<const DerivedType&>(Deref(this)); }
};

#define GP_DefineVec3UnitConstructors(TheUnitName)                                                                                  \
EZ_FORCE_INLINE TheUnitName() {}                                                                                                    \
EZ_FORCE_INLINE explicit TheUnitName(BaseType::Vec3Type value) : BaseType(value) {}                                                 \
EZ_FORCE_INLINE explicit TheUnitName(BaseType::ScalarType x, BaseType::ScalarType y, BaseType::ScalarType z) : BaseType(x, y, z) {} \
EZ_FORCE_INLINE explicit TheUnitName(BaseType::ScalarType value) : BaseType(value) {}                                               \
EZ_FORCE_INLINE explicit TheUnitName(gpZeroInitialization z) : BaseType(z) {}                                                       \
EZ_FORCE_INLINE explicit TheUnitName(gpNoInitialization n) : BaseType(n) {}                                                         \
EZ_FORCE_INLINE TheUnitName(const TheUnitName& other) : BaseType(other) {}

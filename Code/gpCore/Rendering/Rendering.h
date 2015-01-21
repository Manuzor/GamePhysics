#pragma once

namespace gpDrawData
{
    enum class Type
    {
        Point,
        Line,
        Arrow,
        Polygon,
        Sphere,
        Box
    };

    struct Base
    {
        Base(Type type, size_t uiTypeSize) : m_Type(type), m_uiOffsetToNext(uiTypeSize) {}

        const Type m_Type;
        const size_t m_uiOffsetToNext;
    };

    /// \brief Convenient access to the next instance in memory.
    EZ_FORCE_INLINE Base* gpNext(Base* pBase)
    {
        // cast \a pBase to byte-ptr -> add offset -> cast back to Base-ptr
        return reinterpret_cast<Base*>(reinterpret_cast<gpByte*>(pBase) + pBase->m_uiOffsetToNext);
    }

    /// \brief Helper struct to declare draw data types more conveniently
    template<typename Derived, Type DerivedType>
    struct ExtendBaseHelper : public Base
    {
        ExtendBaseHelper() : Base(DerivedType, sizeof(Derived)) {}
    };

    struct Point : public ExtendBaseHelper<Point, Type::Point>
    {
        gpVec3 m_Position;

        float m_fPointSize = 1.0f;
        ezColor m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Line : public ExtendBaseHelper<Line, Type::Line>
    {
        gpVec3 m_Start;
        gpVec3 m_End;

        float m_fLineWidth = 1.0f;
        ezColor m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Arrow : public ExtendBaseHelper<Arrow, Type::Arrow>
    {
        gpVec3 m_Start;
        gpVec3 m_End;

        float m_fLineWidth = 1.0f;
        ezColor m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

        /// \brief The angle from the arrow body to the arrow wings.
        /// \example
        ///   \
        /// --->
        ///   /
        /// => The angle between --- and / but NOT \ and /
        ezAngle m_WingAngle = ezAngle::Degree(30);

        /// \brief The scale of the arrow wings shape in relation to the arrow body.
        float m_fWingLength = 10.0f;
    };

    struct Polygon : public ExtendBaseHelper<Polygon, Type::Polygon>
    {
        gpTransform m_Transform = gpTransform(gpIdentity);
        ezHybridArray<gpVec3, 8> m_Vertices;

        ezColor m_FillColor =    { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_fOutlineWidth = 1.0f;
        ezColor m_OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Sphere : public ExtendBaseHelper<Sphere, Type::Sphere>
    {
        gpVec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_fRadius = 1.0f;
        ezUInt32 m_uiNumSegments = 10;

        ezColor m_FillColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_fOutlineWidth = 1.0f;
        ezColor m_OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Box : public ExtendBaseHelper<Box, Type::Box>
    {
        gpTransform m_Transform = gpTransform(gpIdentity);
        gpRectI m_Box = { 0, 0, 0, 0 }; ///< Relative to m_Transform.

        ezColor m_FillColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_fOutlineWidth = 1.0f;
        ezColor m_OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    };
}

void gpDraw(const gpDrawData::Point& Data);
void gpDraw(const gpDrawData::Line& Data);
void gpDraw(const gpDrawData::Arrow& Data);
void gpDraw(const gpDrawData::Polygon& Data);
void gpDraw(const gpDrawData::Sphere& Data);
void gpDraw(const gpDrawData::Box& Data);

#if EZ_ENABLED(GP_UseOpenGL)
#include "gpCore/Rendering/OpenGL/OpenGL.h"
#elif EZ_ENABLED(GP_UseDirect3D)
#include "gpCore/Rendering/Direct3D/Direct3D.h"
#endif

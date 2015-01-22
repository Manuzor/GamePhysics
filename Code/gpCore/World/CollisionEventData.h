#pragma once
#include "gpCore/Utilities/Pair.h"

class gpCollisionEventData
{
public: // Data types
    enum Type
    {
        Detected,
        Resolved,
    };

private: // Data
    Type m_type;
    gpEntity* m_pFirst;
    gpEntity* m_pSecond;

public: // Algorithms / Friends
    EZ_FORCE_INLINE friend Type gpTypeOf(const gpCollisionEventData& evt) { return evt.m_type; }

    EZ_FORCE_INLINE friend       gpEntity& gpFirstEntityOf(      gpCollisionEventData& evt) { return Deref(evt.m_pFirst); }
    EZ_FORCE_INLINE friend const gpEntity& gpFirstEntityOf(const gpCollisionEventData& evt) { return Deref(evt.m_pFirst); }

    EZ_FORCE_INLINE friend       gpEntity& gpSecondEntityOf(      gpCollisionEventData& evt) { return Deref(evt.m_pSecond); }
    EZ_FORCE_INLINE friend const gpEntity& gpSecondEntityOf(const gpCollisionEventData& evt) { return Deref(evt.m_pSecond); }

    EZ_FORCE_INLINE friend       gpEntity*& gpFirstEntityPtrOf(      gpCollisionEventData& evt) { return evt.m_pFirst; }
    EZ_FORCE_INLINE friend const gpEntity*  gpFirstEntityPtrOf(const gpCollisionEventData& evt) { return evt.m_pFirst; }

    EZ_FORCE_INLINE friend       gpEntity*& gpSecondEntityPtrOf(      gpCollisionEventData& evt) { return evt.m_pSecond; }
    EZ_FORCE_INLINE friend const gpEntity*  gpSecondEntityPtrOf(const gpCollisionEventData& evt) { return evt.m_pSecond; }

public: // Construction
    gpCollisionEventData(Type type, gpEntity* pFirst, gpEntity* pSecond) :
        m_type(type),
        m_pFirst(pFirst),
        m_pSecond(pSecond)
    {
    }

    gpCollisionEventData(const gpCollisionEventData&) = delete;
};

EZ_FORCE_INLINE gpPair<gpEntity*&, gpEntity*&> gpCollisionPairOf(gpCollisionEventData& evt)
{
    return { gpFirstEntityPtrOf(evt), gpSecondEntityPtrOf(evt) };
}

EZ_FORCE_INLINE gpPair<const gpEntity*, const gpEntity*> gpCollisionPairOf(const gpCollisionEventData& evt)
{
    return { gpFirstEntityPtrOf(evt), gpSecondEntityPtrOf(evt) };
}

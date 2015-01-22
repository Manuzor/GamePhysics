#pragma once
#include "gpCore/Utilities/Pair.h"

class gpCollisionEventArgs
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
    EZ_FORCE_INLINE friend Type gpTypeOf(const gpCollisionEventArgs& evt) { return evt.m_type; }

    EZ_FORCE_INLINE friend       gpEntity& gpFirstEntityOf(      gpCollisionEventArgs& evt) { return Deref(evt.m_pFirst); }
    EZ_FORCE_INLINE friend const gpEntity& gpFirstEntityOf(const gpCollisionEventArgs& evt) { return Deref(evt.m_pFirst); }

    EZ_FORCE_INLINE friend       gpEntity& gpSecondEntityOf(      gpCollisionEventArgs& evt) { return Deref(evt.m_pSecond); }
    EZ_FORCE_INLINE friend const gpEntity& gpSecondEntityOf(const gpCollisionEventArgs& evt) { return Deref(evt.m_pSecond); }

    EZ_FORCE_INLINE friend       gpEntity*& gpFirstEntityPtrOf(      gpCollisionEventArgs& evt) { return evt.m_pFirst; }
    EZ_FORCE_INLINE friend const gpEntity*  gpFirstEntityPtrOf(const gpCollisionEventArgs& evt) { return evt.m_pFirst; }

    EZ_FORCE_INLINE friend       gpEntity*& gpSecondEntityPtrOf(      gpCollisionEventArgs& evt) { return evt.m_pSecond; }
    EZ_FORCE_INLINE friend const gpEntity*  gpSecondEntityPtrOf(const gpCollisionEventArgs& evt) { return evt.m_pSecond; }

public: // Construction
    gpCollisionEventArgs(Type type, gpEntity* pFirst, gpEntity* pSecond) :
        m_type(type),
        m_pFirst(pFirst),
        m_pSecond(pSecond)
    {
    }

    gpCollisionEventArgs(const gpCollisionEventArgs&) = delete;
};

EZ_FORCE_INLINE gpPair<gpEntity*&, gpEntity*&> gpCollisionPairOf(gpCollisionEventArgs& evt)
{
    return { gpFirstEntityPtrOf(evt), gpSecondEntityPtrOf(evt) };
}

EZ_FORCE_INLINE gpPair<const gpEntity*, const gpEntity*> gpCollisionPairOf(const gpCollisionEventArgs& evt)
{
    return { gpFirstEntityPtrOf(evt), gpSecondEntityPtrOf(evt) };
}

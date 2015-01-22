#pragma once


/// \brief Stores a pointer to a reference counted object and automatically increases / decreases the reference count.
template <typename T>
class gpSmartPtr
{
public:

    /// \brief Constructor.
    gpSmartPtr() : m_pReferencedObject(nullptr)
    {
    }

    /// \brief Constructor, increases the ref count of the given object.
    gpSmartPtr(T* pReferencedObject) : m_pReferencedObject(pReferencedObject)
    {
        AddReferenceIfValid();
    }

    gpSmartPtr(const gpSmartPtr<T>& Other)
    {
        m_pReferencedObject = Other.m_pReferencedObject;

        AddReferenceIfValid();
    }

    /// \brief Destructor - releases the reference on the ref-counted object (if there is one).
    ~gpSmartPtr()
    {
        ReleaseReferenceIfValid();
    }

    /// \brief Assignment operator, decreases the ref count of the currently referenced object and increases the ref count of the newly assigned object.
    void operator = (T* pNewReference)
    {
        if(pNewReference == m_pReferencedObject)
            return;

        ReleaseReferenceIfValid();

        m_pReferencedObject = pNewReference;

        AddReferenceIfValid();
    }

    /// \brief Assignment operator, decreases the ref count of the currently referenced object and increases the ref count of the newly assigned object.
    void operator = (const gpSmartPtr<T>& Other)
    {
        if(Other.m_pReferencedObject == m_pReferencedObject)
            return;

        ReleaseReferenceIfValid();

        m_pReferencedObject = Other.m_pReferencedObject;

        AddReferenceIfValid();
    }

    /// \brief Returns the referenced object (may be nullptr).
    operator const T*() const
    {
        return m_pReferencedObject;
    }

    /// \brief Returns the referenced object (may be nullptr).
    operator T*()
    {
        return m_pReferencedObject;
    }

    /// \brief Returns the referenced object (may be nullptr).
    const T* operator ->() const
    {
        EZ_ASSERT(m_pReferencedObject != nullptr, "Pointer is nullptr.");
        return m_pReferencedObject;
    }

    /// \brief Returns the referenced object (may be nullptr)
    T* operator -> ()
    {
        EZ_ASSERT(m_pReferencedObject != nullptr, "Pointer is nullptr.");
        return m_pReferencedObject;
    }

private:

    /// \brief Internal helper function to add a reference on the current object (if != nullptr)
    inline void AddReferenceIfValid()
    {
        if(m_pReferencedObject != nullptr)
        {
            gpAddReferenceTo(m_pReferencedObject);
        }
    }

    /// \brief Internal helper function to release a reference on the current object (if != nullptr)
    inline void ReleaseReferenceIfValid()
    {
        if(m_pReferencedObject != nullptr)
        {
            gpReleaseReferenceTo(m_pReferencedObject);
            if (!m_pReferencedObject->IsReferenced())
            {
                gpHandleUnreferencedObject(m_pReferencedObject);
            }
        }
    }

    T* m_pReferencedObject; ///< Stores a pointer to the referenced object
};

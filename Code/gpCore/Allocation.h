#pragma once

/// \brief Default type allocator forwarding all calls to EZ_DEFAULT_NEW[_XYZ].
template<typename T>
struct gpDefaultTypeAllocator
{
    // Default ones
    template<typename... Args>
    EZ_FORCE_INLINE static T* New(Args&&... args) { return EZ_DEFAULT_NEW(T)(args...); }
    EZ_FORCE_INLINE static void Delete(T*& pInstance) { EZ_DEFAULT_DELETE(pInstance); }

    EZ_FORCE_INLINE static ezArrayPtr<T> NewArray(ezUInt32 N) { return EZ_DEFAULT_NEW_ARRAY(T, N); }
    EZ_FORCE_INLINE static void DeleteArray(ezArrayPtr<T>& arr) { EZ_DEFAULT_DELETE_ARRAY(arr); }

    EZ_FORCE_INLINE static T* NewBuffer(ezUInt32 N) { return EZ_DEFAULT_NEW_RAW_BUFFER(T, N); }
    EZ_FORCE_INLINE static void DeleteArray(T*& buffer) { EZ_DEFAULT_DELETE_RAW_BUFFER(buffer); }
};

/// \brief Specialize this template for the types you want to allocate in a special way.
template<typename T>
struct gpTypeAllocator : public gpDefaultTypeAllocator<T>
{
};

template<typename T, typename... Args> EZ_FORCE_INLINE
T* gpNew(Args&&... args)
{
    return gpTypeAllocator<T>::New(args...);
}

template<typename T> EZ_FORCE_INLINE
void gpDelete(T* pInstance)
{
    gpTypeAllocator<T>::Delete(pInstance);
}

template<typename T> EZ_FORCE_INLINE
ezArrayPtr<T> gpNewArray(ezUInt32 N)
{
    return gpTypeAllocator<T>::NewArray(N);
}

template<typename T> EZ_FORCE_INLINE
void gpDeleteArray(ezArrayPtr<T>& arr)
{
    gpTypeAllocator<T>::DeleteArray(arr);
}

/// \brief Triggers the the global event GP_Core_GarbageCollectionEvent
/// \example To add a listener, do the following in some translation unit:
///          EZ_ON_GLOBAL_EVENT(gpCore_GarbageCollectionEvent)
///          {
///              // handle gc ...
///          }
/// \note Do NOT use this macro GP_Core_GarbageCollectionEvent directly, but the string it contains.
//GP_CoreAPI void gpTriggerGarbageCollection();

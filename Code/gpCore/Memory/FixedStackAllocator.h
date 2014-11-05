#pragma once

class gpFixedStackAllocator
{
public:

    gpFixedStackAllocator(ezAllocatorBase* pAllocator, size_t uiSize) { ezMemoryUtils::ZeroFill(this); m_pAllocator = pAllocator; Resize(uiSize); }
    gpFixedStackAllocator(ezAllocatorBase* pAllocator)                { ezMemoryUtils::ZeroFill(this); m_pAllocator = pAllocator; }
    gpFixedStackAllocator()                                           { ezMemoryUtils::ZeroFill(this); m_pAllocator = ezFoundation::GetDefaultAllocator(); }
    ~gpFixedStackAllocator() { Clear(); }

    gpByte* Allocate(size_t uiCount);

    template<typename T>
    T* Allocate(size_t uiCount = 1) { return (T*)Allocate(uiCount * sizeof(T)); }

    /// \brief Resizes the internal data buffer to the given size.
    ///
    /// If the given \a uiSize is smaller than the size of the existing internal buffer,
    /// no operation is performed. If you want to force reallocating, use Clear() before
    /// calling this function.
    void Resize(size_t uiSize);

    /// \brief Deallocates the internal buffer.
    void Clear();

    /// \brief Resets the current allocation pointer to the beginning of the memory block.
    /// \remark Does not deallocate the internal buffer.
    void Reset() { m_pCurrent = m_pBegin; }

          gpByte* GetBegin()       { return m_pBegin; }
    const gpByte* GetBegin() const { return m_pBegin; }

          gpByte* GetEnd()       { return m_pEnd; }
    const gpByte* GetEnd() const { return m_pEnd; }

          gpByte* GetCurrent()       { return m_pCurrent; }
    const gpByte* GetCurrent() const { return m_pCurrent; }

    size_t GetTotalSize()     const { return m_pEnd     - m_pBegin; }
    size_t GetAllocatedSize() const { return m_pCurrent - m_pBegin; }
    size_t GetAvailableSize() const { return m_pEnd     - m_pCurrent; }

    void SetAllocator(ezAllocatorBase* pAllocator);

private:
    ezAllocatorBase* m_pAllocator;
    gpByte* m_pBegin;
    gpByte* m_pEnd;
    gpByte* m_pCurrent;
};

#include "gpCore/Memory/Implementation/FixedStackAllocator.inl"

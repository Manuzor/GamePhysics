
EZ_FORCE_INLINE gpByte* gpFixedStackAllocator::Allocate(size_t uiSize)
{
    EZ_ASSERT(m_pBegin, "");
    EZ_ASSERT(m_pCurrent + uiSize < m_pEnd, "");

    auto pResult = m_pCurrent;
    m_pCurrent += uiSize;
    return pResult;
}

EZ_FORCE_INLINE void gpFixedStackAllocator::Resize(size_t uiSize)
{
    EZ_ASSERT(m_pAllocator, "");

    // If we already have the required capacity, don't do anything.
    if (GetTotalSize() >= uiSize)
        return;

    if(m_pBegin) m_pAllocator->Deallocate(m_pBegin);
    m_pBegin = (gpByte*)m_pAllocator->Allocate(uiSize, 1);
    m_pCurrent = m_pBegin;
    m_pEnd = m_pBegin + uiSize;
}

EZ_FORCE_INLINE void gpFixedStackAllocator::Clear()
{
    if(m_pBegin) m_pAllocator->Deallocate(m_pBegin);
    m_pBegin = nullptr;
    m_pCurrent = nullptr;
    m_pEnd = nullptr;
}

EZ_FORCE_INLINE void gpFixedStackAllocator::SetAllocator(ezAllocatorBase* pAllocator)
{
    EZ_ASSERT(m_pBegin == nullptr, "Can only set alloctor after Clear or before Resize was called!");
    m_pAllocator = pAllocator;
}


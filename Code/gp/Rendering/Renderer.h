#pragma once

class gpRenderExtractor;

class gpRenderer
{
public:
    gpRenderer(const gpRenderer&) = delete;
    void operator=(const gpRenderer&) = delete;

    gpRenderer() {}

    void AddExtractor(gpRenderExtractor* pExtractor)
    {
        EZ_ASSERT(!m_extractors.Contains(pExtractor), "Cannot add same extractor twice.");
        m_extractors.PushBack(pExtractor);
    }

    void RemoveExtractor(gpRenderExtractor* pExtractor)
    {
        m_extractors.RemoveSwap(pExtractor);
    }

    void Render();

private:
    ezDynamicArray<gpRenderExtractor*> m_extractors;
};

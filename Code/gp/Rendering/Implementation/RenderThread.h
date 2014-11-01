#pragma once

namespace gpInternal
{
    class gpRenderThread : public ezThread
    {
    public:
        gpRenderThread();
        virtual ezUInt32 Run() override;
        void Stop();
    private:
        bool m_bStop;
    };
}

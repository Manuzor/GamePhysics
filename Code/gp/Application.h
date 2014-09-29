#pragma once
#include <Core/Application/Application.h>
#include <Foundation/Memory/Allocator.h>

class gpApplication : public ezApplication
{
public:

    gpApplication();
    ~gpApplication();

    virtual void AfterEngineInit() override;

    virtual void BeforeEngineShutdown() override;

    virtual ApplicationExecution Run() override;

private:
    ezAllocatorBase* m_pMainAllocator;
};

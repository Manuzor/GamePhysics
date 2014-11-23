#include "gpCore/PCH.h"
#include "gpCore/Utilities/FileSystem.h"

#include <Foundation/IO/OSFile.h>

const char* gpGetWorkingDirectory()
{
    static char szBuffer[MAX_PATH];
    auto iNumRequiredBufferSize = GetCurrentDirectoryA(MAX_PATH, szBuffer);
    if (iNumRequiredBufferSize > MAX_PATH - 1) // -1 due to null terminator '\0'
    {
        // Should probably not happen? ...
        GP_NotImplemented;
    }

    return szBuffer;
}

const char* gpGetApplicationDirectory()
{
    return ezOSFile::GetApplicationDirectory();
}

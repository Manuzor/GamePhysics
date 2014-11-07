#pragma once

namespace ezLogWriter
{
    class GP_CoreAPI TextFile
    {
    public:
        TextFile();
        ~TextFile() { EndLog(); }

        void BeginLog(const char* szFileName);
        void EndLog();

        void LogMessageHandler(const ezLoggingEventData& eventData);

    private:
        ezFileWriter m_File;
        ezString m_BlockBegin;
        ezString m_BlockEnd;
    };
}

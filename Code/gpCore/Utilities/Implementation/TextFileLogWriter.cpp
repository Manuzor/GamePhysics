#include "gpCore/PCH.h"
#include "gpCore/Utilities/TextFileLogWriter.h"

static const char* g_MessageTypeNames[] = {
    "       ", ///< Used for padding.
    "  Error", ///< ErrorMsg          = 1, // An error message.
    "Serious", ///< SeriousWarningMsg = 2, // A serious warning message.
    "Warning", ///< WarningMsg        = 3, // A warning message.
    "Success", ///< SuccessMsg        = 4, // A success message.
    "   Info", ///< InfoMsg           = 5, // An info message.
    "    Dev", ///< DevMsg            = 6, // A development message.
    "  Debug", ///< DebugMsg          = 7, // A debug message.
};

static const char* g_ShortMessageTypeNames[] = {
    "   ", ///< Used for padding.
    "Err", ///< ErrorMsg          = 1, // An error message.
    "Srs", ///< SeriousWarningMsg = 2, // A serious warning message.
    "Wrn", ///< WarningMsg        = 3, // A warning message.
    "Suc", ///< SuccessMsg        = 4, // A success message.
    "Ifo", ///< InfoMsg           = 5, // An info message.
    "Dev", ///< DevMsg            = 6, // A development message.
    "Dbg", ///< DebugMsg          = 7, // A debug message.
};

ezLogWriter::TextFile::TextFile() :
    m_BlockBegin(">>"),
    m_BlockEnd("<<")
{
}

void ezLogWriter::TextFile::BeginLog(const char* szFileName)
{
    EZ_VERIFY(m_File.Open(szFileName).Succeeded(), "Unable to open file %s", szFileName);
    auto sAbsFilePath = m_File.GetFilePathAbsolute();
    m_File.WriteBytes(sAbsFilePath.GetData(),
                      sAbsFilePath.GetElementCount());
    m_File.WriteBytes("\n", 1);
}

void ezLogWriter::TextFile::EndLog()
{
    m_File.Close();
}

void ezLogWriter::TextFile::LogMessageHandler(const ezLoggingEventData& eventData)
{
    if (!m_File.IsOpen())
        return;

    ezStringBuilder sbText;
    static const char* szPrefixFormat = "%s | ";

    if (eventData.m_EventType > ezLogMsgType::None && eventData.m_EventType < ezLogMsgType::All)
    {
        sbText.AppendFormat(szPrefixFormat, g_ShortMessageTypeNames[eventData.m_EventType]);
    }
    else
    {
        sbText.AppendFormat(szPrefixFormat, g_ShortMessageTypeNames[0]);
    }

    auto indentation = eventData.m_uiIndentation;
    while(indentation > 0)
    {
        sbText.Append("  ");
        --indentation;
    }

    bool bFlushWriteCache = false;

    switch (eventData.m_EventType)
    {
    case ezLogMsgType::BeginGroup:
        sbText.AppendFormat("%s %s", m_BlockBegin.GetData(), eventData.m_szText);
        if(!ezStringUtils::IsNullOrEmpty(eventData.m_szTag))
        {
            sbText.AppendFormat(": %s", eventData.m_szTag);
        }
        break;
    case ezLogMsgType::EndGroup:
        sbText.AppendFormat("%s %s", m_BlockEnd.GetData(), eventData.m_szText);
        break;
    case ezLogMsgType::ErrorMsg:
    case ezLogMsgType::SeriousWarningMsg:
        bFlushWriteCache = true;
    case ezLogMsgType::WarningMsg:
    case ezLogMsgType::SuccessMsg:
    case ezLogMsgType::InfoMsg:
    case ezLogMsgType::DevMsg:
    case ezLogMsgType::DebugMsg:
        sbText.AppendFormat("%s", eventData.m_szText);
        break;
    default:
        EZ_REPORT_FAILURE("Unknown message type %d", eventData.m_EventType);
        break;
    }

    if (!sbText.IsEmpty())
    {
        sbText.Append('\n');
        m_File.WriteBytes(sbText.GetData(), sizeof(char) * sbText.GetElementCount());
    }

    if (bFlushWriteCache || true)
        m_File.Flush();
}

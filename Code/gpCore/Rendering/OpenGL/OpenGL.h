namespace gpInternal
{
    struct OpenGLBeginBeginEnd
    {
        OpenGLBeginBeginEnd(GLenum Mode) { glBegin(Mode); }
        ~OpenGLBeginBeginEnd()           { glEnd(); }
    };

    struct OpenGLScopeEnableDisable
    {
        OpenGLScopeEnableDisable(GLenum Mode) : m_Mode(Mode) { glEnable(m_Mode); }
        ~OpenGLScopeEnableDisable()                          { glDisable(m_Mode); }

        GLenum m_Mode;
    };
}

#define GP_OpenGLScope_BeginEnd gpInternal::OpenGLBeginBeginEnd EZ_CONCAT(_OpenGLScope_, EZ_SOURCE_LINE)
#define GP_OpenGLScope_EnableDisable gpInternal::OpenGLScopeEnableDisable EZ_CONCAT(_OpenGLScope_, EZ_SOURCE_LINE)

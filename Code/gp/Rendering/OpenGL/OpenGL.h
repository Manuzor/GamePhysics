
namespace gpDrawData
{
    struct Point
    {
        gpVec3 m_Position;

        float m_fPointSize = 1.0f;
        ezColor m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Line
    {
        gpVec3 m_Start;
        gpVec3 m_End;

        float m_fLineWidth = 1.0f;
        ezColor m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Polygon
    {
        ezArrayPtr<gpVec3> m_Vertices;

        ezColor m_FillColor =    { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_fOutlineWidth = 1.0f;
        ezColor m_OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Circle
    {
        gpVec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_fRadius = 1.0f;
        ezUInt32 m_uiNumLineSegments = 10;

        ezColor m_FillColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_fOutlineWidth = 1.0f;
        ezColor m_OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct Box
    {
        gpRect m_Box;

        ezColor m_FillColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        float m_fOutlineWidth = 1.0f;
        ezColor m_OutlineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    };
}

void gpDraw(const gpDrawData::Point& Data);
void gpDraw(const gpDrawData::Line& Data);
void gpDraw(const gpDrawData::Polygon& Data);
void gpDraw(const gpDrawData::Circle& Data);

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

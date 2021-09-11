#include"GLRenderable.h"

void GLRenderable::Init()
{
    m_ShaderVert.Init("GLDefault", sType::VERT);
    m_ShaderFrag.Init("GLDefault", sType::FRAG);

    const char* vertShaderSource = m_ShaderVert.m_ShaderCode.data();
    const char* fragShaderSource = m_ShaderFrag.m_ShaderCode.data();

    uint32_t vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShaderID, 1, &vertShaderSource, nullptr);
    glCompileShader(vertShaderID);

    glShaderSource(fragShaderID, 1, &fragShaderSource, nullptr);
    glCompileShader(fragShaderID);

    m_ShaderProgramID = glCreateProgram();
    glAttachShader(m_ShaderProgramID, vertShaderID);
    glAttachShader(m_ShaderProgramID, fragShaderID);
    glLinkProgram(m_ShaderProgramID);

    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);
    m_ShaderVert.Destroy();
    m_ShaderFrag.Destroy();

    glGenVertexArrays(1, &m_VAOID);
    glGenBuffers(1, &m_VBOID);
    glGenBuffers(1, &m_EBOID);

    glBindVertexArray(m_VAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
}

void GLRenderable::Clear()
{
    glDeleteProgram(m_ShaderProgramID);

    glDeleteVertexArrays(1, &m_VAOID);
    glDeleteBuffers(1, &m_VBOID);
    glDeleteBuffers(1, &m_EBOID);
}

void GLRenderable::Draw()
{
    glUseProgram(m_ShaderProgramID);
}
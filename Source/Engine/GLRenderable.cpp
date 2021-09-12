#include"GLRenderable.h"

void GLRenderable::Init(std::string Model, std::string DiffuseTexture, Color DiffuseColor)
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

    float groundVertices[] =
    {
         -8.0f, 0.0f, 8.0f,
         -8.0f, 0.0f,-8.0f,
          8.0f, 0.0f,-8.0f,

         -8.0f, 0.0f, 8.0f,
          8.0f, 0.0f,-8.0f,
          8.0f, 0.0f, 8.0f,
    };

    glGenVertexArrays(1, &m_VAOID);
    glGenBuffers(1, &m_VBOID);
    glGenBuffers(1, &m_EBOID);

    glBindVertexArray(m_VAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void GLRenderable::Clear()
{
    glDeleteProgram(m_ShaderProgramID);

    glDeleteVertexArrays(1, &m_VAOID);

    glDeleteBuffers(1, &m_VBOID);
    glDeleteBuffers(1, &m_EBOID);
}

void GLRenderable::Draw(glm::mat4* MVP)
{
    glUseProgram(m_ShaderProgramID);
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "model"),      1, GL_FALSE, glm::value_ptr(MVP[0]));
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "view"),       1, GL_FALSE, glm::value_ptr(MVP[1]));
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(MVP[2]));

    glBindVertexArray(m_VAOID);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
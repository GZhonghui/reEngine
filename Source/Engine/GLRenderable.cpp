#include"GLRenderable.h"

void GLRenderable::Init(const std::string& Model, const std::string& DiffuseTexture, Color DiffuseColor)
{
    uint32_t vertShaderID = GLMisc::CompileShader(Shader("GLDefault", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t fragShaderID = GLMisc::CompileShader(Shader("GLDefault", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_ShaderProgramID = glCreateProgram();
    glAttachShader(m_ShaderProgramID, vertShaderID);
    glAttachShader(m_ShaderProgramID, fragShaderID);
    glLinkProgram(m_ShaderProgramID);

    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    m_DiffuseTextureID = GLMisc::GenDefaultTextureWithImageFile((G_IMPORT_PATH + DiffuseTexture).c_str());

    m_DiffuseColor = DiffuseColor;
    
    // Location UV : 5 Float per Vertex
    std::vector<float> VBuffer;
    std::vector<unsigned int> EBuffer;

    // Load Model
    Loader::ModelLoader modelLoader;
    if (modelLoader.Load((G_IMPORT_PATH + Model).c_str()))
    {
        for (auto meshIndex = modelLoader.m_Meshs.begin(); meshIndex != modelLoader.m_Meshs.end(); ++meshIndex)
        {
            int VertexNumber = VBuffer.size() / 5;
            for (auto faceIndex = (*meshIndex)->Faces.begin(); faceIndex != (*meshIndex)->Faces.end(); ++faceIndex)
            {
                EBuffer.push_back((*faceIndex)->x() + VertexNumber);
                EBuffer.push_back((*faceIndex)->y() + VertexNumber);
                EBuffer.push_back((*faceIndex)->z() + VertexNumber);
            }

            for (auto vertexIndex = (*meshIndex)->Vertices.begin(); vertexIndex != (*meshIndex)->Vertices.end(); ++vertexIndex)
            {
                VBuffer.push_back((*vertexIndex)->m_Location.x());
                VBuffer.push_back((*vertexIndex)->m_Location.y());
                VBuffer.push_back((*vertexIndex)->m_Location.z());
                VBuffer.push_back((*vertexIndex)->m_TextureCoords.x());
                VBuffer.push_back((*vertexIndex)->m_TextureCoords.y());
            }
        }
    }
    else
    {
        Out::Log(pType::ERROR, "Load Model File Failed");
    }

    m_ElementCount = EBuffer.size();

    glGenVertexArrays(1, &m_VAOID);
    glGenBuffers(1, &m_VBOID);
    glGenBuffers(1, &m_EBOID);

    glBindVertexArray(m_VAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VBuffer.size(), VBuffer.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * EBuffer.size(), EBuffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_NeedClear = true;
}

void GLRenderable::Clear()
{
    if (m_NeedClear)
    {
        glDeleteProgram(m_ShaderProgramID);

        glDeleteVertexArrays(1, &m_VAOID);

        glDeleteBuffers(1, &m_VBOID);
        glDeleteBuffers(1, &m_EBOID);

        glDeleteTextures(1, &m_DiffuseTextureID);

        m_NeedClear = false;
    }
}

void GLRenderable::Draw(glm::mat4* MVP)
{
    glUseProgram(m_ShaderProgramID);
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0]));
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));

    glUniform1i(glGetUniformLocation(m_ShaderProgramID, "diffuseTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DiffuseTextureID);

    glBindVertexArray(m_VAOID);
    glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
}
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

    glGenTextures(1, &m_DiffuseTextureID);
    glBindTexture(GL_TEXTURE_2D, m_DiffuseTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string texturePath = std::string(G_IMPORT_PATH) + DiffuseTexture;
    int textureWidth, textureHeight, textureChannels;
    unsigned char* diffuseTextureData = stbi_load(texturePath.c_str(), &textureWidth, &textureHeight, &textureChannels, 3);

    if (!diffuseTextureData)
    {
        Out::Log(pType::ERROR, "Load Texture Failed");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, diffuseTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(diffuseTextureData);

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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DiffuseTextureID);

    glBindVertexArray(m_VAOID);
    glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
}
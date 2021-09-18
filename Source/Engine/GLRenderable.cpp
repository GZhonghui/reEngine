#include"GLRenderable.h"

void GLRenderable::reLoadModel(const std::string& Model)
{
    if (m_VAOID) glDeleteVertexArrays(1, &m_VAOID);
    if (m_VBOID) glDeleteBuffers(1, &m_VBOID);
    if (m_EBOID) glDeleteBuffers(1, &m_EBOID);

    // Location UV : 8 Float per Vertex
    std::vector<float> VBuffer;
    std::vector<unsigned int> EBuffer;

    // Load Model
    Loader::ModelLoader modelLoader;
    if (modelLoader.Load((G_IMPORT_PATH + Model).c_str()))
    {
        for (auto meshIndex = modelLoader.m_Meshs.begin(); meshIndex != modelLoader.m_Meshs.end(); ++meshIndex)
        {
            int VertexNumber = VBuffer.size() / 8;
            for (auto faceIndex = (*meshIndex)->Faces.begin(); faceIndex != (*meshIndex)->Faces.end(); ++faceIndex)
            {
                EBuffer.push_back((*faceIndex)->x() + VertexNumber);
                EBuffer.push_back((*faceIndex)->y() + VertexNumber);
                EBuffer.push_back((*faceIndex)->z() + VertexNumber);
            }

            auto pV = &(*meshIndex)->Vertices;
            for (auto vertexIndex = pV->begin(); vertexIndex != pV->end(); ++vertexIndex)
            {
                VBuffer.push_back((*vertexIndex)->m_Location.x());
                VBuffer.push_back((*vertexIndex)->m_Location.y());
                VBuffer.push_back((*vertexIndex)->m_Location.z());
                VBuffer.push_back((*vertexIndex)->m_Normal.x());
                VBuffer.push_back((*vertexIndex)->m_Normal.y());
                VBuffer.push_back((*vertexIndex)->m_Normal.z());
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void GLRenderable::reLoadDiffuseTexture(const std::string& DiffuseTextureName)
{
    if (m_DiffuseTextureID)  glDeleteTextures(1, &m_DiffuseTextureID);
    m_DiffuseTextureID = GLMisc::GenDefaultTextureWithImageFile((G_IMPORT_PATH + DiffuseTextureName).c_str());
}

void GLRenderable::reLoadNormalTexture(const std::string& NormalTextureName)
{
    if (m_NormalTextureID)   glDeleteTextures(1, &m_NormalTextureID);
    m_NormalTextureID = GLMisc::GenDefaultTextureWithImageFile((G_IMPORT_PATH + NormalTextureName).c_str());
}

void GLRenderable::reLoadSpecularTexture(const std::string& SpecularTextureName)
{
    if (m_SpecularTextureID) glDeleteTextures(1, &m_SpecularTextureID);
    m_SpecularTextureID = GLMisc::GenDefaultTextureWithImageFile((G_IMPORT_PATH + SpecularTextureName).c_str());
}

void GLRenderable::Clear()
{
    if (m_VAOID) glDeleteVertexArrays(1, &m_VAOID);
    if (m_VBOID) glDeleteBuffers(1, &m_VBOID);
    if (m_EBOID) glDeleteBuffers(1, &m_EBOID);

    if (m_DiffuseTextureID)  glDeleteTextures(1, &m_DiffuseTextureID);
    if (m_NormalTextureID)   glDeleteTextures(1, &m_NormalTextureID);
    if (m_SpecularTextureID) glDeleteTextures(1, &m_SpecularTextureID);
}

void GLRenderable::Draw()
{
    if (!m_VAOID) return;
    glBindVertexArray(m_VAOID);
    glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
}
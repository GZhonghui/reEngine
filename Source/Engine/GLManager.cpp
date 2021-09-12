#include"GLManager.h"

void GLManager::InitLight()
{
    // See World Setting
    /*
    m_LightDir = Direction(1, -1, 1);
    m_LightColor = Color(1, 1, 1);
    m_LightPower = 2;
    */
}

void GLManager::InitSkybox()
{
    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    int textureWidth, textureHeight, textureChannels;
    unsigned char* textureData;

    const std::string SkyboxPath("../Asset/Skybox/Creek/");

    textureData = stbi_load((SkyboxPath + "posX.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "negX.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "posZ.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "negZ.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "posY.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    textureData = stbi_load((SkyboxPath + "negY.jpg").c_str(), &textureWidth, &textureHeight, &textureChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free(textureData);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Shader skyboxVertShader("GLSkybox", sType::VERT);
    Shader skyboxFragShader("GLSkybox", sType::FRAG);

    const char* skyboxVertShaderSource = skyboxVertShader.m_ShaderCode.data();
    const char* skyboxFragShaderSource = skyboxFragShader.m_ShaderCode.data();

    uint32_t skyboxVertShaderID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t skyboxFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int  compileSuccessfully;
    char compileInfo[512];

    glShaderSource(skyboxVertShaderID, 1, &skyboxVertShaderSource, nullptr);
    glCompileShader(skyboxVertShaderID);

    glGetShaderiv(skyboxVertShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(skyboxVertShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Skybox Vert Shader Compile Failed : %s", compileInfo);
    }

    glShaderSource(skyboxFragShaderID, 1, &skyboxFragShaderSource, nullptr);
    glCompileShader(skyboxFragShaderID);

    glGetShaderiv(skyboxFragShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
    if (!compileSuccessfully)
    {
        glGetShaderInfoLog(skyboxFragShaderID, 512, NULL, compileInfo);
        Out::Log(pType::WARNING, "Skybox Frag Shader Compile Failed : %s", compileInfo);
    }

    m_SkyboxShaderProgramID = glCreateProgram();
    glAttachShader(m_SkyboxShaderProgramID, skyboxVertShaderID);
    glAttachShader(m_SkyboxShaderProgramID, skyboxFragShaderID);
    glLinkProgram(m_SkyboxShaderProgramID);

    glDeleteShader(skyboxVertShaderID);
    glDeleteShader(skyboxFragShaderID);

    float skyboxVertices[] =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_SkyboxVAOID);
    glGenBuffers(1, &m_SkyboxVBOID);

    glBindVertexArray(m_SkyboxVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void GLManager::DestroySkybox()
{
    glDeleteTextures(1, &m_SkyboxTextureID);
    glDeleteProgram(m_SkyboxShaderProgramID);

    glDeleteVertexArrays(1, &m_SkyboxVAOID);
    glDeleteBuffers(1, &m_SkyboxVBOID);
}

void GLManager::RenderSkybox(uint32_t viewWidth, uint32_t viewHeight,
    const Point& mainCameraLocation, const Direction& mainCameraDir)
{
    glDepthMask(GL_FALSE);

    glUseProgram(m_SkyboxShaderProgramID);

    auto projectionLocation = glGetUniformLocation(m_SkyboxShaderProgramID, "projection");
    auto viewLocation = glGetUniformLocation(m_SkyboxShaderProgramID, "view");

    glm::vec3 cameraLocation = Convert(mainCameraLocation);
    glm::vec3 cameraTarget = Convert(mainCameraLocation + mainCameraDir);

    auto projection = glm::perspective(glm::radians(60.0f), (float)viewWidth / viewHeight, 0.1f, 100.0f);
    auto view = glm::mat4(glm::mat3(glm::lookAt(cameraLocation, cameraTarget, glm::vec3(0, 1, 0))));

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "skybox"), 0);

    glBindVertexArray(m_SkyboxVAOID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}

bool GLManager::Init()
{
    glGenFramebuffers(1, &m_SceneFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    glGenTextures(1, &m_SceneTextureID);

    glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTextureID, 0);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        Out::Log(pType::ERROR, "Init FBO Failed");

        return false;
    }

    InitLight();
    InitSkybox();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Out::Log(pType::MESSAGE, "Inited OpenGL");

    return true;
}

bool GLManager::Destroy()
{
    glDeleteFramebuffers(1, &m_SceneFBO);

    DestroySkybox();

    Out::Log(pType::MESSAGE, "Cleaned OpenGL");

    return true;
}

void GLManager::BeginRenderEditor(uint32_t viewWidth, uint32_t viewHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glViewport(0, 0, viewWidth, viewHeight);

    glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLManager::EndRenderEditor()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLManager::BeginRenderGame(uint32_t viewWidth, uint32_t viewHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, viewWidth, viewHeight);

    glClearColor(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLManager::EndRenderGame()
{

}

void GLManager::Render(std::shared_ptr<GLRenderable> renderObj)
{

}
#include"GLManager.h"

void GLManager::InitSkybox()
{
    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    int texWidth, texHeight, texChannels;
    unsigned char* texData;

    const std::string SkyboxPath("../Asset/Skybox/Creek/");

    texData = stbi_load((SkyboxPath + "posX.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negX.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negZ.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "posZ.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "posY.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negY.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    uint32_t skyboxVertShaderID = GLMisc::CompileShader(Shader("GLSkybox", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t skyboxFragShaderID = GLMisc::CompileShader(Shader("GLSkybox", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_SkyboxShaderProgramID = glCreateProgram();
    glAttachShader(m_SkyboxShaderProgramID, skyboxVertShaderID);
    glAttachShader(m_SkyboxShaderProgramID, skyboxFragShaderID);
    glLinkProgram (m_SkyboxShaderProgramID);

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

void GLManager::RenderSkybox()
{
    glDepthMask(GL_FALSE);

    glUseProgram(m_SkyboxShaderProgramID);

    glm::vec3 cameraLocation = Convert(m_CameraLocation);
    glm::vec3 cameraTarget = Convert(m_CameraLocation + m_CameraDir);

    auto P = glm::perspective(glm::radians(Aspect), (float)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    // Skybox's View is Different
    auto V = glm::mat4(glm::mat3(glm::lookAt(cameraLocation, cameraTarget, glm::vec3(0, 1, 0))));

    glUniformMatrix4fv(glGetUniformLocation(m_SkyboxShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(glGetUniformLocation(m_SkyboxShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(P));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "skyboxTexture"), 0);

    glBindVertexArray(m_SkyboxVAOID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}

void GLManager::InitDefaultScene()
{
    m_DefaultSceneTextureID = GLMisc::GenDefaultTextureWithImageFile("../Asset/Texture/UV_Texture.png");

    uint32_t groundVertShaderID = GLMisc::CompileShader(Shader("GLDefault", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t groundFragShaderID = GLMisc::CompileShader(Shader("GLDefault", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_DefaultSceneShaderProgramID = glCreateProgram();
    glAttachShader(m_DefaultSceneShaderProgramID, groundVertShaderID);
    glAttachShader(m_DefaultSceneShaderProgramID, groundFragShaderID);
    glLinkProgram (m_DefaultSceneShaderProgramID);

    glDeleteShader(groundVertShaderID);
    glDeleteShader(groundFragShaderID);

    int repeatCount = 1;
    float groundVertices[] =
    {
         -10.0f, 0.0f, 10.0f, 0.0f, 0.0f,
         -10.0f, 0.0f,-10.0f, 0.0f, repeatCount,
          10.0f, 0.0f,-10.0f, repeatCount, repeatCount,

         -10.0f, 0.0f, 10.0f, 0.0f, 0.0f,
          10.0f, 0.0f,-10.0f, repeatCount, repeatCount,
          10.0f, 0.0f, 10.0f, repeatCount, 0.0f
    };

    glGenVertexArrays(1, &m_DefaultSceneVAOID);
    glGenBuffers(1, &m_DefaultSceneVBOID);

    glBindVertexArray(m_DefaultSceneVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_DefaultSceneVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GLManager::DestroyDefaultScene()
{
    glDeleteTextures(1, &m_DefaultSceneTextureID);
    glDeleteProgram(m_DefaultSceneShaderProgramID);

    glDeleteVertexArrays(1, &m_DefaultSceneVAOID);
    glDeleteBuffers(1, &m_DefaultSceneVBOID);
}

void GLManager::RenderDefaultScene()
{
    glUseProgram(m_DefaultSceneShaderProgramID);

    auto M = glm::mat4(1);
    
    glm::mat4 V;
    Event::mainCamera.updateView(V);
    
    auto P = glm::perspective(glm::radians(Aspect), (float)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    glUniformMatrix4fv(glGetUniformLocation(m_DefaultSceneShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(M));
    glUniformMatrix4fv(glGetUniformLocation(m_DefaultSceneShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(glGetUniformLocation(m_DefaultSceneShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(P));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "diffuseTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DefaultSceneTextureID);

    glBindVertexArray(m_DefaultSceneVAOID);

    glDrawArrays(GL_TRIANGLES, 0, 6);
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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTextureID, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &m_DepthTextureID);
    glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 128, 128, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        Out::Log(pType::ERROR, "Init FBO Failed");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    InitSkybox();
    InitDefaultScene();

    Out::Log(pType::MESSAGE, "Inited OpenGL");

    return true;
}

bool GLManager::Destroy()
{
    glDeleteFramebuffers(1, &m_SceneFBO);

    glDeleteTextures(1, &m_SceneTextureID);
    glDeleteTextures(1, &m_DepthTextureID);

    DestroySkybox();
    DestroyDefaultScene();

    Out::Log(pType::MESSAGE, "Cleaned OpenGL");

    return true;
}

void GLManager::BeginRenderEditor(uint32_t viewWidth, uint32_t viewHeight,
    const Point& mainCameraLocation, const Direction& mainCameraDir)
{
    m_ViewWidth      = viewWidth;
    m_ViewHeight     = viewHeight;
    m_CameraLocation = mainCameraLocation;
    m_CameraDir      = mainCameraDir;

    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    glEnable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewWidth, viewHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glViewport(0, 0, viewWidth, viewHeight);

    glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLManager::EndRenderEditor()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLManager::BeginRenderGame(uint32_t viewWidth, uint32_t viewHeight,
    const Point& mainCameraLocation, const Direction& mainCameraDir)
{
    m_ViewWidth      = viewWidth;
    m_ViewHeight     = viewHeight;
    m_CameraLocation = mainCameraLocation;
    m_CameraDir      = mainCameraDir;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, viewWidth, viewHeight);

    glClearColor(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLManager::EndRenderGame()
{
    // End Render Game
}

void GLManager::Render(std::shared_ptr<GLRenderable> renderObj, const Transform& ObjTransform)
{
    glm::mat4 MVP[3];

    // Transform
    MVP[0] = glm::mat4(1);
    MVP[1] = glm::translate(MVP[0], Convert(ObjTransform.Location));

    Event::mainCamera.updateView(MVP[1]);

    MVP[2] = glm::perspective(glm::radians(Aspect), (float)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    renderObj->Draw(MVP);
}
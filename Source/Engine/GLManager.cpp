#include"GLManager.h"

void GLManager::Skybox::Init()
{
    // HARD CODE
    m_Skyboxs.push_back("Creek");
    m_SkyboxsPath.push_back("../Asset/Skybox/Creek/");
    m_RevSkyboxs[std::string("Creek")] = 0;

    m_Skyboxs.push_back("Water");
    m_SkyboxsPath.push_back("../Asset/Skybox/Water/");
    m_RevSkyboxs[std::string("Water")] = 1;

    m_SkyboxsChar = "Creek\0Water\0";
    // HARD CODE

    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_NowSkybox = -1;
    Change(0);

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

void GLManager::Skybox::Destroy()
{
    glDeleteTextures(1, &m_SkyboxTextureID);
    glDeleteProgram (m_SkyboxShaderProgramID);

    glDeleteVertexArrays(1, &m_SkyboxVAOID);
    glDeleteBuffers(1, &m_SkyboxVBOID);
}

void GLManager::Skybox::Render(glm::mat4* VP)
{
    glDepthMask(GL_FALSE);

    glUseProgram(m_SkyboxShaderProgramID);

    // Skybox's View is Different
    auto skyboxV = glm::mat4(glm::mat3(VP[0]));

    glUniformMatrix4fv(glGetUniformLocation(m_SkyboxShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(skyboxV));
    glUniformMatrix4fv(glGetUniformLocation(m_SkyboxShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(VP[1]));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "skyboxTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glBindVertexArray(m_SkyboxVAOID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}

void GLManager::Skybox::Change(int Which)
{
    if (!Inside(Which, 0, m_Skyboxs.size() - 1)) return;
    if (m_NowSkybox == Which) return;

    std::string SkyboxPath = m_SkyboxsPath[Which];

    int texWidth, texHeight, texChannels;
    unsigned char* texData;

    // Don't Need On Cubemap
    stbi_set_flip_vertically_on_load(false);
    texData = stbi_load((SkyboxPath + "posX.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negX.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "posY.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negY.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "posZ.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negZ.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);
    stbi_set_flip_vertically_on_load(true);

    m_NowSkybox = Which;
}

void GLManager::Grid::Init()
{
    uint32_t groundVertShaderID = GLMisc::CompileShader(Shader("GLLine", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t groundFragShaderID = GLMisc::CompileShader(Shader("GLLine", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_GridShaderProgramID = glCreateProgram();
    glAttachShader(m_GridShaderProgramID, groundVertShaderID);
    glAttachShader(m_GridShaderProgramID, groundFragShaderID);
    glLinkProgram (m_GridShaderProgramID);

    glDeleteShader(groundVertShaderID);
    glDeleteShader(groundFragShaderID);

    glGenVertexArrays(1, &m_GridVAOID);
    glGenBuffers(1, &m_GridVBOID);

    std::vector<float> V;

    const int   GridSize   = 100;
    const float GridOffset = 0.1;
    const float GridStart  =  GridSize * -0.5 * GridOffset;
    const float GridEnd    = -GridStart;
    
    for (int wIndex = 0; wIndex <= GridSize; wIndex += 1)
    {
        V.push_back(GridStart + wIndex * GridOffset);
        V.push_back(0);
        V.push_back(GridStart);

        V.push_back(GridStart + wIndex * GridOffset);
        V.push_back(0);
        V.push_back(GridEnd);
    }

    for (int hIndex = 0; hIndex <= GridSize; hIndex += 1)
    {
        V.push_back(GridStart);
        V.push_back(0);
        V.push_back(GridStart + hIndex * GridOffset);
        
        V.push_back(GridEnd);
        V.push_back(0);
        V.push_back(GridStart + hIndex * GridOffset);
    }

    m_GridVerticesCount = V.size() / 3;

    glBindVertexArray(m_GridVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_GridVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * V.size(), V.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void GLManager::Grid::Destroy()
{
    glDeleteProgram(m_GridShaderProgramID);

    glDeleteVertexArrays(1, &m_GridVAOID);
    glDeleteBuffers(1, &m_GridVBOID);
}

void GLManager::Grid::Render(glm::mat4* VP)
{
    glUseProgram(m_GridShaderProgramID);

    auto M = glm::mat4(1);

    glUniformMatrix4fv(glGetUniformLocation(m_GridShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(M));
    glUniformMatrix4fv(glGetUniformLocation(m_GridShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(VP[0]));
    glUniformMatrix4fv(glGetUniformLocation(m_GridShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(VP[1]));
    glUniform3f(glGetUniformLocation(m_GridShaderProgramID, "lineColor"), colorGray.x(), colorGray.y(), colorGray.z());

    glLineWidth(1.0);

    glBindVertexArray(m_GridVAOID);
    glDrawArrays(GL_LINES, 0, m_GridVerticesCount);
}

void GLManager::Axis::Init()
{
    uint32_t axisEndVertShaderID = GLMisc::CompileShader(Shader("GLAxisEnd", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t axisEndFragShaderID = GLMisc::CompileShader(Shader("GLAxisEnd", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_AxisEndShaderProgramID = glCreateProgram();
    glAttachShader(m_AxisEndShaderProgramID, axisEndVertShaderID);
    glAttachShader(m_AxisEndShaderProgramID, axisEndFragShaderID);
    glLinkProgram (m_AxisEndShaderProgramID);

    glDeleteShader(axisEndVertShaderID);
    glDeleteShader(axisEndFragShaderID);

    uint32_t axisLineVertShaderID = GLMisc::CompileShader(Shader("GLAxisLine", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t axisLineFragShaderID = GLMisc::CompileShader(Shader("GLAxisLine", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_AxisLineShaderProgramID = glCreateProgram();
    glAttachShader(m_AxisLineShaderProgramID, axisLineVertShaderID);
    glAttachShader(m_AxisLineShaderProgramID, axisLineFragShaderID);
    glLinkProgram (m_AxisLineShaderProgramID);

    glDeleteShader(axisLineVertShaderID);
    glDeleteShader(axisLineFragShaderID);

    float boxVertices[] =
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

    glGenVertexArrays(1, &m_AxisEndVAOID);
    glGenBuffers(1, &m_AxisEndVBOID);

    glBindVertexArray(m_AxisEndVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_AxisEndVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float lineVertices[] =
    {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_AxisLineVAOID);
    glGenBuffers(1, &m_AxisLineVBOID);

    glBindVertexArray(m_AxisLineVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_AxisLineVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GLManager::Axis::Destroy()
{
    glDeleteProgram(m_AxisEndShaderProgramID);
    glDeleteProgram(m_AxisLineShaderProgramID);

    glDeleteVertexArrays(1, &m_AxisEndVAOID);
    glDeleteVertexArrays(1, &m_AxisLineVAOID);
    glDeleteBuffers(1, &m_AxisEndVBOID);
    glDeleteBuffers(1, &m_AxisLineVBOID);
}

void GLManager::Axis::Render(double Length, double Size, glm::mat4* MVP)
{
    glDisable(GL_DEPTH_TEST);

    glUseProgram(m_AxisEndShaderProgramID);

    auto SomeAxis = [&](glm::vec3 Which)
    {
        glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(Size));
        glm::mat4 T = glm::translate(glm::mat4(1), Which * (float)Length);

        glUniformMatrix4fv(glGetUniformLocation(m_AxisEndShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0] * T * S));
        glUniformMatrix4fv(glGetUniformLocation(m_AxisEndShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
        glUniformMatrix4fv(glGetUniformLocation(m_AxisEndShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));
        glUniform3f(glGetUniformLocation(m_AxisEndShaderProgramID, "axisColor"), Which.x, Which.y, Which.z);

        glBindVertexArray(m_AxisEndVAOID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    };

    SomeAxis(glm::vec3(1, 0, 0));
    SomeAxis(glm::vec3(0, 1, 0));
    SomeAxis(glm::vec3(0, 0, 1));

    glUseProgram(m_AxisLineShaderProgramID);

    glUniformMatrix4fv(glGetUniformLocation(m_AxisLineShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0]));
    glUniformMatrix4fv(glGetUniformLocation(m_AxisLineShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
    glUniformMatrix4fv(glGetUniformLocation(m_AxisLineShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));
    glUniform1f(glGetUniformLocation(m_AxisLineShaderProgramID, "lineLength"), Length);

    glLineWidth(3);

    glBindVertexArray(m_AxisLineVAOID);
    glDrawArrays(GL_LINES, 0, 6);

    glEnable(GL_DEPTH_TEST);
}

void GLManager::Outline::Init()
{
    uint32_t outlineVertShaderID = GLMisc::CompileShader(Shader("GLOutline", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t outlineFragShaderID = GLMisc::CompileShader(Shader("GLOutline", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_OutlineShaderID = glCreateProgram();
    glAttachShader(m_OutlineShaderID, outlineVertShaderID);
    glAttachShader(m_OutlineShaderID, outlineFragShaderID);
    glLinkProgram (m_OutlineShaderID);

    glDeleteShader(outlineVertShaderID);
    glDeleteShader(outlineFragShaderID);
}

void GLManager::Outline::Destroy()
{
    glDeleteProgram(m_OutlineShaderID);
}

void GLManager::ShaderManager::Init()
{
    // HARD CODE
    m_SupportShaders.push_back("Default");
    m_SupportShadersPath.push_back("GLDefault");
    m_RevSupportShaders[std::string("Default")] = 0;

    m_SupportShaders.push_back("Glass");
    m_SupportShadersPath.push_back("GLGlass");
    m_RevSupportShaders[std::string("Glass")] = 1;

    m_SupportShaders.push_back("Metal");
    m_SupportShadersPath.push_back("GLMetal");
    m_RevSupportShaders[std::string("Metal")] = 2;


    m_SupportShaders.push_back("Cartoon");
    m_SupportShadersPath.push_back("GLCartoon");
    m_RevSupportShaders[std::string("Cartoon")] = 3;

    m_SupportShadersChar = "Default\0Glass\0Metal\0Cartoon\0";
    // HARD CODE

    auto loadAGroupShader = [](const std::string& ShaderPath)->uint32_t
    {
        uint32_t vertShader = GLMisc::CompileShader(Shader(ShaderPath.c_str(), sType::VERT).m_ShaderCode.data(), sType::VERT);
        uint32_t fragShader = GLMisc::CompileShader(Shader(ShaderPath.c_str(), sType::FRAG).m_ShaderCode.data(), sType::FRAG);

        uint32_t ShaderProgramID = glCreateProgram();
        glAttachShader(ShaderProgramID, vertShader);
        glAttachShader(ShaderProgramID, fragShader);
        glLinkProgram (ShaderProgramID);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return ShaderProgramID;
    };
    
    // HARD CODE
    m_DefaultShaderProgramID = loadAGroupShader(m_SupportShadersPath[0]);
    m_GlassShaderProgramID   = loadAGroupShader(m_SupportShadersPath[1]);
    m_MetalShaderProgramID   = loadAGroupShader(m_SupportShadersPath[2]);
    m_CartoonShaderProgramID = loadAGroupShader(m_SupportShadersPath[3]);
    // HARD CODE
}

void GLManager::ShaderManager::Destroy()
{
    glDeleteProgram(m_DefaultShaderProgramID);
    glDeleteProgram(m_GlassShaderProgramID);
    glDeleteProgram(m_MetalShaderProgramID);
    glDeleteProgram(m_CartoonShaderProgramID);
}

bool GLManager::Init()
{
    glGenFramebuffers(1, &m_SceneFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    glGenTextures(1, &m_SceneTextureID);
    glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    
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

    m_Skybox.Init();
    m_Grid.Init();
    m_Axis.Init();
    m_Outline.Init();
    m_ShaderManager.Init();

    Out::Log(pType::MESSAGE, "Inited OpenGL");

    return true;
}

bool GLManager::Destroy()
{
    glDeleteFramebuffers(1, &m_SceneFBO);

    glDeleteTextures(1, &m_SceneTextureID);
    glDeleteTextures(1, &m_DepthTextureID);

    m_Skybox.Destroy();
    m_Grid.Destroy();
    m_Axis.Destroy();
    m_Outline.Destroy();
    m_ShaderManager.Destroy();

    Out::Log(pType::MESSAGE, "Cleaned OpenGL");

    return true;
}

void GLManager::BeginRenderEditor(uint32_t viewW, uint32_t viewH, const Point& CameraL, const Direction& CameraD)
{
    m_ViewWidth      = viewW;
    m_ViewHeight     = viewH;
    m_CameraLocation = CameraL;
    m_CameraDir      = CameraD;

    m_P = glm::perspective(glm::radians(Aspect), (double)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    glEnable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, viewW, viewH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewW, viewH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glViewport(0, 0, viewW, viewH);

    glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLManager::EndRenderEditor()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLManager::BeginRenderGame(uint32_t viewW, uint32_t viewH, const Point& CameraL, const Direction& CameraD)
{
    m_ViewWidth      = viewW;
    m_ViewHeight     = viewH;
    m_CameraLocation = CameraL;
    m_CameraDir      = CameraD;

    m_P = glm::perspective(glm::radians(Aspect), (double)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, viewW, viewH);

    glClearColor(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLManager::EndRenderGame()
{
    // End Render Game
}

void GLManager::Render
(
    std::shared_ptr<GLRenderable> renderObj,
    const Transform& ObjTransform, bool Selected, bool LineMode
)
{
    if (!renderObj) return;

    glm::mat4 MVP[3];

    glm::mat4 ModelWithoutScale;

    // Transform
    MVP[0] = glm::mat4(1);
    MVP[0] = glm::translate(MVP[0], Convert(ObjTransform.Location));
    MVP[0] = glm::rotate(MVP[0], glm::radians((float)ObjTransform.Rotation.x()), glm::vec3(1, 0, 0));
    MVP[0] = glm::rotate(MVP[0], glm::radians((float)ObjTransform.Rotation.y()), glm::vec3(0, 1, 0));
    MVP[0] = glm::rotate(MVP[0], glm::radians((float)ObjTransform.Rotation.z()), glm::vec3(0, 0, 1));

    ModelWithoutScale = MVP[0];

    MVP[0] = glm::scale(MVP[0], Convert(ObjTransform.Scale));

    Event::mainCamera.updateView(MVP[1]);

    MVP[2] = m_P;

    if (LineMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), glLineWidth(1);

    unsigned int usedShaderID = 0;

    if (renderObj->m_Shader == "Default")
    {
        glUseProgram(m_ShaderManager.m_DefaultShaderProgramID);
        usedShaderID = m_ShaderManager.m_DefaultShaderProgramID;

        glUniform3f(glGetUniformLocation(usedShaderID, "lightDir"), m_LightDir.x(), m_LightDir.y(), m_LightDir.z());
        glUniform3f(glGetUniformLocation(usedShaderID, "lightColor"), m_LightColor.x(), m_LightColor.y(), m_LightColor.z());
        glUniform1f(glGetUniformLocation(usedShaderID, "lightPower"), m_LightPower);

        auto pColor = &renderObj->m_DiffuseColor;
        glUniform3f(glGetUniformLocation(usedShaderID, "diffuseColor"), pColor->x(), pColor->y(), pColor->z());

        if (renderObj->m_EnableDiffuseTexture)
        {
            glUniform1i(glGetUniformLocation(usedShaderID, "enableDiffuseTexture"), 1);
            glUniform1i(glGetUniformLocation(usedShaderID, "diffuseTexture"), 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderObj->m_DiffuseTextureID);
        }
        else
        {
            glUniform1i(glGetUniformLocation(usedShaderID, "enableDiffuseTexture"), 0);
        }

        if (renderObj->m_EnableNormalTexture)
        {
            glUniform1i(glGetUniformLocation(usedShaderID, "enableNormalTexture"), 1);
            glUniform1i(glGetUniformLocation(usedShaderID, "normalTexture"), 1);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, renderObj->m_NormalTextureID);
        }
        else
        {
            glUniform1i(glGetUniformLocation(usedShaderID, "enableNormalTexture"), 0);
        }

        if (renderObj->m_EnableSpecularTexture)
        {
            glUniform1i(glGetUniformLocation(usedShaderID, "enableSpecularTexture"), 1);
            glUniform1i(glGetUniformLocation(usedShaderID, "specularTexture"), 2);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, renderObj->m_SpecularTextureID);
        }
        else
        {
            glUniform1i(glGetUniformLocation(usedShaderID, "enableSpecularTexture"), 0);
        }
    }
    else if (renderObj->m_Shader == "Glass")
    {
        glUseProgram(m_ShaderManager.m_GlassShaderProgramID);
        usedShaderID = m_ShaderManager.m_GlassShaderProgramID;

        glUniform3f(glGetUniformLocation(usedShaderID, "cameraPos"),
            m_CameraLocation.x(), m_CameraLocation.y(), m_CameraLocation.z());
        glUniform1i(glGetUniformLocation(usedShaderID, "skyboxTexture"), 0);
        glUniform1f(glGetUniformLocation(usedShaderID, "N"), renderObj->m_N);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.m_SkyboxTextureID);
    }
    else if (renderObj->m_Shader == "Metal")
    {
        glUseProgram(m_ShaderManager.m_MetalShaderProgramID);
        usedShaderID = m_ShaderManager.m_MetalShaderProgramID;

        glUniform3f(glGetUniformLocation(usedShaderID, "cameraPos"),
            m_CameraLocation.x(), m_CameraLocation.y(), m_CameraLocation.z());
        glUniform1i(glGetUniformLocation(usedShaderID, "skyboxTexture"), 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.m_SkyboxTextureID);
    }
    else if (renderObj->m_Shader == "Cartoon")
    {
        glUseProgram(m_ShaderManager.m_CartoonShaderProgramID);
        usedShaderID = m_ShaderManager.m_CartoonShaderProgramID;

        auto pColor = &renderObj->m_DiffuseColor;
        glUniform3f(glGetUniformLocation(usedShaderID, "diffuseColor"), pColor->x(), pColor->y(), pColor->z());

        glUniform3f(glGetUniformLocation(usedShaderID, "lightDir"), m_LightDir.x(), m_LightDir.y(), m_LightDir.z());
    }
    else
    {
        return;
    }

    glUniformMatrix4fv(glGetUniformLocation(usedShaderID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0]));
    glUniformMatrix4fv(glGetUniformLocation(usedShaderID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
    glUniformMatrix4fv(glGetUniformLocation(usedShaderID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));

    renderObj->Draw();

    if (LineMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (Selected)
    {
        MVP[0] = ModelWithoutScale;
        m_Axis.Render(1, 0.1, MVP);
    }
}
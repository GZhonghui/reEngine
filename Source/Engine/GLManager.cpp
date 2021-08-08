#include"GLManager.h"

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Out::Log(pType::MESSAGE, "Inited OpenGL");

    return true;
}

bool GLManager::Destroy()
{
    glDeleteFramebuffers(1, &m_SceneFBO);

    Out::Log(pType::MESSAGE, "Cleaned OpenGL");

    return true;
}

unsigned int GLManager::RenderScene(uint32_t viewWidth, uint32_t viewHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glViewport(0, 0, viewWidth, viewHeight);

    glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return m_SceneTextureID;
}

void GLManager::RenderGame(uint32_t viewWidth, uint32_t viewHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, viewWidth, viewHeight);

    glClearColor(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
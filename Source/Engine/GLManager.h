#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLRenderable.h"
#include"Event.h"

class GLManager
{
public:
    GLManager() = default;
    virtual ~GLManager() = default;

private:
    const double NearZ  = 0.100;
    const double FarZ   = 300.0;
    const double Aspect = 60.00;

protected:
    uint32_t m_SceneFBO;
    uint32_t m_SceneTextureID;
    uint32_t m_DepthTextureID;

public:
    uint32_t getSceneTextureID() const { return m_SceneTextureID; }

protected:
    // Update Every Frame
    uint32_t m_ViewWidth;
    uint32_t m_ViewHeight;
    
    Point     m_CameraLocation;
    Direction m_CameraDir;
    glm::mat4 m_P;

protected:
    Direction m_LightDir;
    Color     m_LightColor;
    double    m_LightPower;

public:
    Direction getLightDir() const noexcept         { return m_LightDir;       }
    void      setLightDir(const Direction& newDir) { m_LightDir = newDir;     }

    Color     getLightColor() const noexcept       { return m_LightColor;     }
    void      setLightColor(const Color& newColor) { m_LightColor = newColor; }

    double    getLightPower() const noexcept       { return m_LightPower;     }
    void      setLightPower(double newPower)       { m_LightPower = newPower; }

protected:
    class Skybox
    {
    public:
        Skybox()
        {
            m_NowSkybox = 0;
        }
        virtual ~Skybox() = default;

    public:
        uint32_t m_SkyboxTextureID;
        uint32_t m_SkyboxShaderProgramID;
        uint32_t m_SkyboxVAOID;
        uint32_t m_SkyboxVBOID;

        const char* m_SkyboxsChar;
        std::vector<std::string> m_Skyboxs;
        std::vector<std::string> m_SkyboxsPath;
        std::unordered_map<std::string, int> m_RevSkyboxs;

        int m_NowSkybox;

    public:
        void Init();
        void Destroy();
        void Render(glm::mat4* VP);
        void Change(int Which);
    }m_Skybox;

public:
    void RenderSkybox()
    {
        glm::mat4 VP[2];
        
        Event::mainCamera.updateView(VP[0]);

        VP[1] = m_P;

        m_Skybox.Render(VP);
    }

    void ChangeSkybox(int Which)
    {
        m_Skybox.Change(Which);
    }

    unsigned int getNowSkybox()   const { return m_Skybox.m_NowSkybox;      }
    const char*  getSkyboxList()  const { return m_Skybox.m_SkyboxsChar;    }
    unsigned int getSkyboxCount() const { return m_Skybox.m_Skyboxs.size(); }
    std::string  getSkyboxAt(int Idx)   { return m_Skybox.m_Skyboxs[Idx];   }
    int          getIdxOfSkybox(const std::string& inStr)
    {
        if (!m_Skybox.m_RevSkyboxs.count(inStr)) return -1;
        return m_Skybox.m_RevSkyboxs[inStr];
    }

protected:
    class Grid
    {
    public:
        Grid() = default;
        virtual ~Grid() = default;

    public:
        uint32_t m_GridShaderProgramID;
        uint32_t m_GridVAOID;
        uint32_t m_GridVBOID;

        unsigned int m_GridVerticesCount;

    public:
        void Init();
        void Destroy();
        void Render(glm::mat4* VP);
    }m_Grid;

public:
    void RenderGrid()
    {
        glm::mat4 VP[2];

        Event::mainCamera.updateView(VP[0]);

        VP[1] = m_P;

        m_Skybox.Render(VP);
    }

protected:
    class Axis
    {
    public:
        Axis() = default;
        virtual ~Axis() = default;

    public:
        uint32_t m_AxisEndShaderProgramID;
        uint32_t m_AxisEndVAOID;
        uint32_t m_AxisEndVBOID;

        uint32_t m_AxisLineShaderProgramID;
        uint32_t m_AxisLineVAOID;
        uint32_t m_AxisLineVBOID;

    public:
        void Init();
        void Destroy();
        void Render(double Length, double Size, glm::mat4* MVP);
    }m_Axis;

protected:
    class Outline
    {
    public:
        Outline() = default;
        virtual ~Outline() = default;

    public:
        uint32_t m_OutlineShaderID;

    public:
        void Init();
        void Destroy();
    }m_Outline;

protected:
    class ShaderManager
    {
    public:
        ShaderManager() = default;
        virtual ~ShaderManager() = default;

    public:
        uint32_t m_DefaultShaderProgramID;
        uint32_t m_GlassShaderProgramID;

        const char* m_SupportShadersChar;
        std::vector<std::string> m_SupportShaders;
        std::vector<std::string> m_SupportShadersPath;
        std::unordered_map<std::string, int> m_RevSupportShaders;

    public:
        void Init();
        void Destroy();
    }m_ShaderManager;

public:
    const char*  getSupportShaderList()  const { return m_ShaderManager.m_SupportShadersChar;    }
    unsigned int getSupportShaderCount() const { return m_ShaderManager.m_SupportShaders.size(); }
    std::string  getSupportShaderAt(int Idx)   { return m_ShaderManager.m_SupportShaders[Idx];   }
    int          getIdxOfSupportShader(const std::string& inStr)
    {
        if (!m_ShaderManager.m_RevSupportShaders.count(inStr)) return -1;
        return m_ShaderManager.m_RevSupportShaders[inStr];
    }

public:
    bool Init();
    bool Destroy();

public:
    void BeginRenderEditor(uint32_t viewW, uint32_t viewH, const Point& CameraL, const Direction& CameraD);
    void EndRenderEditor();

    void BeginRenderGame(uint32_t viewW, uint32_t viewH, const Point& CameraL, const Direction& CameraD);
    void EndRenderGame();

public:
    void Render
    (
        std::shared_ptr<GLRenderable> renderObj,
        const Transform& ObjTransform, bool Selected, bool LineMode
    );
};
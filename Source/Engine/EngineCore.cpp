#include"EngineCore.h"

//From class Event
Camera Event::mainCamera;

bool Event::shouldQuit;
bool Event::windowResized;
bool Event::mouseAsCursor;

float Event::cameraMoveSpeed;

namespace EngineCore
{
    GLFWwindow* mainWindow;
    GLManager glManager;

    // Game Mode Only
    std::vector<std::shared_ptr<Actor>> actorsInSceneOfGame;

    std::unordered_map<std::string, std::shared_ptr<GLRenderable>> classesInSceneForRender;

    WorldSetting worldSettings;
    std::vector<ClassItem> classItems;
    std::vector<ActorItem> actorItems;
    std::vector<ComponentItem> componentItems;

    // Exchange with UI
    int  coreSelectedActorInEditorScene;
    bool coreRenderGridInEditorScene;
    int  coreRenderModeInEditorScene;

    void initGLFW()
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mainWindow = glfwCreateWindow
        (
            G_MIN_WINDOW_WIDTH, G_MIN_WINDOW_HEIGHT,
            "reEngine", nullptr, nullptr
        );

        if (!mainWindow)
        {
            Out::Log(pType::ERROR, "init GLFW Window Failed");

            return;
        }

        glfwSetWindowSizeLimits
        (
            mainWindow,
            G_MIN_WINDOW_WIDTH, G_MIN_WINDOW_HEIGHT,
            GLFW_DONT_CARE, GLFW_DONT_CARE
        );

        glfwSetKeyCallback(mainWindow, Event::glfwKeyCallback);
        glfwSetCursorPosCallback(mainWindow, Event::glfwCursorPositionCallback);
        glfwSetMouseButtonCallback(mainWindow, Event::glfwMouseButtonCallback);
        glfwSetWindowSizeCallback(mainWindow, Event::glfwWindowResizeCallback);

        glfwMakeContextCurrent(mainWindow);
        glfwSwapInterval(1);

        gladLoadGL();
    }

    void initImGuiForGL()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsClassic();

        ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void initOpenGL()
    {
        glManager.Init();
    }

    void destroyGLFW()
    {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }

    void destroyImGuiForGL()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void destroyOpenGL()
    {
        glManager.Destroy();
    }

    uint32_t RenderEditorScene(uint32_t viewWidth, uint32_t viewHeight)
    {
        glManager.BeginRenderEditor(viewWidth, viewHeight, Event::getCameraLocation(), Event::getCameraDir());

        glManager.RenderSkybox();

        if (coreRenderGridInEditorScene)
        {
            glManager.RenderGrid();
        }

        int actorIndex = 0;
        for (auto i = actorItems.begin(); i != actorItems.end(); ++i)
        {
            if (classesInSceneForRender.count(i->m_ClassName) && classesInSceneForRender[i->m_ClassName])
            {
                glManager.Render(classesInSceneForRender[i->m_ClassName],
                    Transform(i->m_Location, i->m_Rotation, i->m_Scale),
                    actorIndex == coreSelectedActorInEditorScene,
                    coreRenderModeInEditorScene);
            }
            ++actorIndex;
        }

        glManager.EndRenderEditor();

        return glManager.getSceneTextureID();
    }

    void RenderGame()
    {
        int nowWidth, nowHeight;
        glfwGetFramebufferSize(mainWindow, &nowWidth, &nowHeight);

        glManager.BeginRenderGame(nowWidth, nowHeight, Event::getCameraLocation(), Event::getCameraDir());

        glManager.RenderSkybox();
        glManager.RenderGrid();

        glManager.EndRenderGame();
    }

    void Render()
    {
        glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (G_BUILD_GAME_MODE)
        {
            RenderGame();
        }
        else
        {
            RenderEditor();
        }
    }

    void initActors()
    {
        for (auto thisAcotr = actorsInSceneOfGame.begin(); thisAcotr != actorsInSceneOfGame.end(); ++thisAcotr)
        {
            thisAcotr->get()->Init();
        }
    }

    void destroyActors()
    {
        for (auto thisAcotr = actorsInSceneOfGame.begin(); thisAcotr != actorsInSceneOfGame.end(); ++thisAcotr)
        {
            thisAcotr->get()->Destroy();
        }
    }

    void Update()
    {
        static bool firstUpdate = true;
        static double lastUpdateTime = glfwGetTime();
        
        double thisUpdateTime = glfwGetTime();

        float Delta = static_cast<float>(thisUpdateTime - lastUpdateTime);

        lastUpdateTime = thisUpdateTime;

        if (firstUpdate)
        {
            firstUpdate = false;
            return;
        }

        for (auto aIndex = actorsInSceneOfGame.begin(); aIndex != actorsInSceneOfGame.end(); ++aIndex)
        {
            aIndex->get()->Update(Delta);
        }
    }

    void applyWorldSettings()
    {
        glManager.setLightDir(worldSettings.m_LightDir);
        glManager.setLightColor(worldSettings.m_LightColor);
        glManager.setLightPower(worldSettings.m_LightPower);

        glManager.ChangeSkybox(glManager.getIdxOfSkybox(worldSettings.m_Skybox));
    }

    void collectWorldSettings()
    {
        worldSettings.m_LightDir = glManager.getLightDir();
        worldSettings.m_LightColor = glManager.getLightColor();
        worldSettings.m_LightPower = glManager.getLightPower();

        worldSettings.m_Skybox = glManager.getSkyboxAt(glManager.getNowSkybox());
    }
};

int engineMain(void (*initScene)(std::vector<std::shared_ptr<Actor>>* actorsInScene))
{
    // Important
    stbi_set_flip_vertically_on_load(true);

    EngineCore::initGLFW();
    EngineCore::initImGuiForGL();
    EngineCore::initOpenGL();

    Event::initEventState();

    readProject
    (
        EngineCore::worldSettings, EngineCore::actorItems,
        EngineCore::classItems, EngineCore::componentItems
    );
    EngineCore::applyWorldSettings();

    if (G_BUILD_GAME_MODE)
    {
        EngineCore::actorsInSceneOfGame.clear();
        initScene(&EngineCore::actorsInSceneOfGame);

        EngineCore::initActors();
    }

    while (!glfwWindowShouldClose(EngineCore::mainWindow))
    {
        glfwPollEvents();
        Event::Loop(EngineCore::mainWindow);

        EngineCore::Render();

        if (G_BUILD_GAME_MODE)
        {
            EngineCore::Update();
        }

        glfwSwapBuffers(EngineCore::mainWindow);
    }

    EngineCore::destroyOpenGL();
    EngineCore::destroyImGuiForGL();
    EngineCore::destroyGLFW();

    if (G_BUILD_GAME_MODE)
    {
        EngineCore::destroyActors();
    }

    return 0;
}
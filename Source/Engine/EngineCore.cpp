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

    std::vector<std::shared_ptr<Actor>> actorsInScene;

    std::vector<ClassItem> classItems;
    std::vector<ActorItem> actorItems;

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
        glManager.BeginRenderEditor(viewWidth, viewHeight);

        glManager.RenderSkybox(viewWidth, viewHeight, Event::getCameraLocation(), Event::getCameraDir());

        glManager.EndRenderEditor();

        return glManager.getSceneTextureID();
    }

    void RenderGame()
    {
        int nowWidth, nowHeight;
        glfwGetFramebufferSize(mainWindow, &nowWidth, &nowHeight);

        glManager.BeginRenderGame(nowWidth, nowHeight);

        glManager.RenderSkybox(nowWidth, nowHeight, Event::getCameraLocation(), Event::getCameraDir());

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
        for (auto thisAcotr = actorsInScene.begin(); thisAcotr != actorsInScene.end(); ++thisAcotr)
        {
            thisAcotr->get()->Init();
        }
    }

    void destroyActors()
    {
        for (auto thisAcotr = actorsInScene.begin(); thisAcotr != actorsInScene.end(); ++thisAcotr)
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

        for (auto aIndex = actorsInScene.begin(); aIndex != actorsInScene.end(); ++aIndex)
        {
            aIndex->get()->Update(Delta);
        }
    }

    std::shared_ptr<Actor> getActorByID(unsigned int actorID)
    {
        return nullptr;
    }

    std::shared_ptr<Actor> getActorByName(const std::string& actorName)
    {
        return nullptr;
    }

    std::vector<std::shared_ptr<Actor>> getActorsByTag(const std::string& tagName)
    {
        std::vector<std::shared_ptr<Actor>> findResult;

        return findResult;
    }

    std::vector<std::shared_ptr<Actor>> getActorsByClass(const std::string& className)
    {
        std::vector<std::shared_ptr<Actor>> findResult;

        return findResult;
    }
};

int engineMain(void (*initScene)(std::vector<std::shared_ptr<Actor>>* actorsInScene))
{

    readProject(EngineCore::classItems, EngineCore::actorItems);

    if (G_BUILD_GAME_MODE)
    {
        EngineCore::actorsInScene.clear();
        initScene(&EngineCore::actorsInScene);

        EngineCore::initActors();
    }

    EngineCore::initGLFW();
    EngineCore::initImGuiForGL();
    EngineCore::initOpenGL();

    Event::initEventState();

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
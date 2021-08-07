#include"EngineCore.h"

//From class Event
Camera Event::mainCamera;

bool Event::shouldQuit;
bool Event::moveState;
bool Event::windowResized;

float Event::cameraMoveSpeed;

namespace EngineCore
{
    GLFWwindow* mainWindow;
    GLManager glManager;

    std::vector<std::shared_ptr<Actor>> actorsInScene;

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
            Out::Log(pType::ERROR, "init GLFW Failed");

            return;
        }

        glfwSetWindowSizeLimits
        (
            mainWindow,
            G_MIN_WINDOW_WIDTH, G_MIN_WINDOW_HEIGHT,
            GLFW_DONT_CARE, GLFW_DONT_CARE
        );

        glfwMakeContextCurrent(mainWindow);
        glfwSwapInterval(1);

        gladLoadGL();
    }

    void initImGuiForGL()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

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

    void RenderEditorUI()
    {
        const float leftWindowWidth = 256;
        const float rightWindowWidth = 256;
        const float toolBoxHeight = 96;

        int displayW, displayH;
        glfwGetFramebufferSize(mainWindow, &displayW, &displayH);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto WinFlag = 
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGui::SetNextWindowSize(ImVec2(leftWindowWidth, displayH), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

        if (ImGui::Begin("Actors", nullptr, WinFlag))
        {
            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, displayH), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(displayW - rightWindowWidth, 0), ImGuiCond_Always);

        if (ImGui::Begin("Files", nullptr, WinFlag))
        {
            ImGui::End();
        }

        auto centerWidth = displayW - leftWindowWidth - rightWindowWidth;
        ImGui::SetNextWindowSize(ImVec2(centerWidth, toolBoxHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(leftWindowWidth, 0), ImGuiCond_Always);

        static bool showAddNewClassWindow = false;

        if (ImGui::Begin("Tools", nullptr, WinFlag))
        {
            if (ImGui::Button("Add a Class"))
            {
                showAddNewClassWindow = true;
            }

            ImGui::End();
        }

        if (showAddNewClassWindow)
        {
            if (ImGui::Begin("Add a Class", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
            {
                char newClassName[16];
                memset(newClassName, 0, sizeof(newClassName));

                ImGui::Text("Guide for Create new Class");
                ImGui::Separator();
                ImGui::Text("Input the Class Name");
                ImGui::InputText("", newClassName, 16);

                if (ImGui::Button("Add"))
                {
                    showAddNewClassWindow = false;
                }

                if (ImGui::Button("Cancel"))
                {
                    showAddNewClassWindow = false;
                }

                ImGui::End();
            }
        }

        ImGui::SetNextWindowSize(ImVec2(centerWidth, displayH - toolBoxHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(leftWindowWidth, toolBoxHeight), ImGuiCond_Always);

        if (ImGui::Begin("Scene", nullptr, WinFlag))
        {
            ImGui::End();
        }

        ImGui::Render();

        glViewport(0, 0, displayW, displayH);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void RenderEditorScene()
    {

    }

    void Render()
    {
        glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (G_BUILD_GAME_MODE)
        {

        }
        else
        {
            RenderEditorScene();
            RenderEditorUI();
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
        float Delta = 0;

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
    if (G_BUILD_GAME_MODE)
    {
        EngineCore::actorsInScene.clear();
        initScene(&EngineCore::actorsInScene);

        EngineCore::initActors();
    }

    EngineCore::initGLFW();
    EngineCore::initImGuiForGL();
    EngineCore::initOpenGL();

    while (!glfwWindowShouldClose(EngineCore::mainWindow))
    {
        glfwPollEvents();

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
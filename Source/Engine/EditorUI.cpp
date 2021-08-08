#include"EngineCore.h"

namespace EngineCore
{
    extern GLFWwindow* mainWindow;
    extern GLManager glManager;

    extern std::vector<std::shared_ptr<Actor>> actorsInScene;

    extern std::vector<ClassItem> classItems;
    extern std::vector<ActorItem> actorItems;

    void RenderEditorUI()
    {
        // CONST
        const float leftWindowWidth = 256;
        const float rightWindowWidth = 256;
        const float toolBoxHeight = 96;
        // CONST

        int displayW, displayH;
        glfwGetFramebufferSize(mainWindow, &displayW, &displayH);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto mainWinFlag =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
        auto popWinFlag =
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

        // STATIC
        static uint8_t leftEnableTab = 0;
        static std::vector<const char*> actorItemsChar;
        static int actorCurrent = 0;
        static std::vector<const char*> classItemsChar;
        static int classCurrent = 0;
        // STATIC

        classItemsChar.clear();
        actorItemsChar.clear();

        for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
        {
            classItemsChar.push_back(classIndex->m_Name.c_str());
        }
        for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
        {
            actorItemsChar.push_back(actorIndex->m_Name.c_str());
        }

        int listBoxHeightCount = ((displayH - 64) / ImGui::GetTextLineHeightWithSpacing());
        ImGui::SetNextWindowSize(ImVec2(leftWindowWidth, displayH), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        if (ImGui::Begin("Actors", nullptr, mainWinFlag))
        {
            if (ImGui::BeginTabBar("Actors"))
            {
                if (ImGui::BeginTabItem("Actor List"))
                {
                    leftEnableTab = 0;
                    ImGui::PushItemWidth(-1);
                    ImGui::ListBox("", &actorCurrent, actorItemsChar.data(), actorItemsChar.size(), listBoxHeightCount);
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Class List"))
                {
                    leftEnableTab = 1;
                    ImGui::PushItemWidth(-1);
                    ImGui::ListBox("", &classCurrent, classItemsChar.data(), classItemsChar.size(), listBoxHeightCount);
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, displayH), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(displayW - rightWindowWidth, 0), ImGuiCond_Always);
        if (ImGui::Begin("Details", nullptr, mainWinFlag))
        {
            if (leftEnableTab == 0)
            {
                ImGui::Text("Actor Details");
                ImGui::Separator();
                if (actorCurrent < actorItemsChar.size())
                {
                    ImGui::Text("Actor Name: %s", actorItemsChar[actorCurrent]);
                    ImGui::Text("Actor Type: %s", actorItems[actorCurrent].m_ClassName.c_str());
                }
            }
            else if (leftEnableTab == 1)
            {
                ImGui::Text("Class Details");
                ImGui::Separator();
                if (classCurrent < classItemsChar.size())
                {
                    ImGui::Text("Class Name: %s", classItemsChar[classCurrent]);
                }
            }

            ImGui::End();
        }

        // STATIC
        static bool showAddNewClassWindow = false;
        static bool showAboutWindow = false;
        static char newClassName[16];
        // STATIC

        auto centerWidth = displayW - leftWindowWidth - rightWindowWidth;
        ImGui::SetNextWindowSize(ImVec2(centerWidth, toolBoxHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(leftWindowWidth, 0), ImGuiCond_Always);
        if (ImGui::Begin("Tools", nullptr, mainWinFlag))
        {
            if (ImGui::BeginTabBar("Tools"))
            {
                if (ImGui::BeginTabItem("File"))
                {
                    if (ImGui::Button("Reload Project"))
                    {
                        readProject(classItems, actorItems);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Write Project"))
                    {
                        saveProject(classItems, actorItems);
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Class"))
                {
                    if (ImGui::Button("Add a Class"))
                    {
                        showAddNewClassWindow = true;
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Actor"))
                {
                    if (ImGui::Button("Add a Actor"))
                    {
                        
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Delete Selected Actor"))
                    {

                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Help"))
                {
                    if (ImGui::Button("About"))
                    {
                        showAboutWindow = true;
                    }
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
            ImGui::End();
        }

        if (showAddNewClassWindow)
        {
            if (ImGui::Begin("Add a Class", nullptr, popWinFlag))
            {
                ImGui::Text("Guide for Create new Class");
                ImGui::Separator();
                ImGui::Text("Input the Class Name");
                ImGui::InputText("", newClassName, IM_ARRAYSIZE(newClassName));

                if (ImGui::Button("Add"))
                {
                    if (*newClassName)
                    {
                        addClassToProject(newClassName);

                        ClassItem newClass;
                        newClass.m_Name = std::string(newClassName);
                        classItems.push_back(newClass);

                        showAddNewClassWindow = false;
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    showAddNewClassWindow = false;
                }
                ImGui::End();
            }
        }

        if (showAboutWindow)
        {
            if (ImGui::Begin("About", &showAboutWindow, popWinFlag))
            {
                ImGui::Text("About reEngine");
                ImGui::Separator();
                ImGui::Text("reEngine is an Open Source Game Engine");
                ImGui::Text("https://github.com/GZhonghui/reEngine");
                ImGui::End();
            }
        }

        ImGui::SetNextWindowSize(ImVec2(centerWidth, displayH - toolBoxHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(leftWindowWidth, toolBoxHeight), ImGuiCond_Always);
        if (ImGui::Begin("Scene", nullptr, mainWinFlag))
        {
            ImGui::End();
        }

        ImGui::Render();

        glViewport(0, 0, displayW, displayH);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
#include"EngineCore.h"

namespace EngineCore
{
    extern GLFWwindow* mainWindow;
    extern GLManager glManager;

    extern std::vector<std::shared_ptr<Actor>> actorsInScene;

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

        // STATIC
        static uint8_t leftEnableTab = 0;
        static std::vector<const char*> actorItems;
        static int actorCurrent = 0;
        static std::vector<const char*> classItems;
        static int classCurrent = 0;
        // STATIC

        static std::string a("some text");

        if (actorItems.empty())
        {
            actorItems.push_back(a.c_str());
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
                    ImGui::ListBox("", &actorCurrent, actorItems.data(), actorItems.size(), listBoxHeightCount);
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Class List"))
                {
                    leftEnableTab = 1;
                    ImGui::PushItemWidth(-1);
                    ImGui::ListBox("", &classCurrent, classItems.data(), classItems.size(), listBoxHeightCount);
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
                if (actorCurrent < actorItems.size())
                {
                    ImGui::Text("Actor Name: %s", actorItems[actorCurrent]);
                }
            }
            else if (leftEnableTab == 1)
            {
                ImGui::Text("Class Details");
                ImGui::Separator();
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
            if (ImGui::Begin("Add a Class", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
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
            if (ImGui::Begin("About", &showAboutWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
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

        ImGui::ShowDemoWindow();

        ImGui::Render();

        glViewport(0, 0, displayW, displayH);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
#include"EngineCore.h"

namespace EngineCore
{
    // FROM EngineCore.cpp
    extern GLFWwindow* mainWindow;
    extern GLManager glManager;

    extern std::vector<std::shared_ptr<Actor>> actorsInScene;

    extern std::vector<ClassItem> classItems;
    extern std::vector<ActorItem> actorItems;

    extern std::unordered_set<std::string> classNameSet;
    extern std::unordered_set<std::string> actorNameSet;
    // FROM EngineCore.cpp

    void RenderEditor()
    {
        // CONST
        const float leftWindowWidth = 256;
        const float rightWindowWidth = 256;
        const float toolBoxHeight = 96;
        const float detailHeight = 360;
        // CONST

        // STATIC
        static bool havePopWindow = false;
        // STATIC

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

        ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, detailHeight), ImGuiCond_Always);
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

        ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, displayH - detailHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(displayW - rightWindowWidth, detailHeight), ImGuiCond_Always);
        if (ImGui::Begin("Info", nullptr, mainWinFlag))
        {
            ImGui::Text("Camera");
            auto cameraLocation = Event::getCameraLocation();
            auto cameraDir = Event::getCameraDir();

            ImGui::Text("Location (%.2lf,%.2lf,%.2lf)",
                cameraLocation.x(), cameraLocation.y(), cameraLocation.z());
            ImGui::Text("Forward (%.2lf,%.2lf,%.2lf)",
                cameraDir.x(), cameraDir.y(), cameraDir.z());

            ImGui::End();
        }

        // STATIC
        static bool showAddNewClassWindow = false;
        static bool showAddNewActorWindow = false;
        static bool showAboutWindow = false;
        static char newClassName[16];
        static char newActorName[32];
        static int selectClassInNewActor = 0;
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
                        readProject(classItems, actorItems, classNameSet, actorNameSet);
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
                        if (!havePopWindow)
                        {
                            showAddNewClassWindow = true;
                            havePopWindow = true;
                        }
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Actor"))
                {
                    if (ImGui::Button("Add a Actor"))
                    {
                        if (!havePopWindow)
                        {
                            showAddNewActorWindow = true;
                            havePopWindow = true;
                        }
                        
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

                if (ImGui::Button("Add & Save"))
                {
                    if (checkClassOrActorName(newClassName))
                    {
                        addClassToProject(newClassName);

                        ClassItem newClass;
                        newClass.m_Name = std::string(newClassName);
                        classItems.push_back(newClass);

                        saveProject(classItems, actorItems);

                        showAddNewClassWindow = false;
                        havePopWindow = false;
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    showAddNewClassWindow = false;
                    havePopWindow = false;
                }
                ImGui::End();
            }
        }

        if (showAddNewActorWindow)
        {
            if (ImGui::Begin("Add a Actor", nullptr, popWinFlag))
            {
                ImGui::Text("Guide for Create new Actor");
                ImGui::Separator();

                if (classItemsChar.empty())
                {
                    ImGui::Text("Please Add a Class First");
                }
                else
                {
                    ImGui::Text("Input the Actor Name");
                    ImGui::InputText("", newActorName, IM_ARRAYSIZE(newActorName));
                    ImGui::Text("Selete the Class");
                    ImGui::Combo("", &selectClassInNewActor, classItemsChar.data(), classItemsChar.size());

                    if (ImGui::Button("Add"))
                    {
                        if (checkClassOrActorName(newActorName) && selectClassInNewActor < classItemsChar.size())
                        {
                            ActorItem newActor;
                            newActor.m_Name = std::string(newActorName);
                            newActor.m_ClassName = classItems[selectClassInNewActor].m_Name;

                            actorItems.push_back(newActor);

                            showAddNewActorWindow = false;
                            havePopWindow = false;
                        }
                    }
                    ImGui::SameLine();
                }

                if (ImGui::Button("Cancel"))
                {
                    showAddNewActorWindow = false;
                    havePopWindow = false;
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
            auto sceneWindowSize = ImGui::GetWindowSize();
            sceneWindowSize.x -= 16;
            sceneWindowSize.y -= 36;

            auto sceneTextureID = RenderEditorScene(sceneWindowSize.x, sceneWindowSize.y);
            ImGui::Image((void*)sceneTextureID, sceneWindowSize, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
        }

        ImGui::Render();

        glViewport(0, 0, displayW, displayH);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
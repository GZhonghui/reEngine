#include"EngineCore.h"

// FROM ToolAIO.cpp
extern std::vector<std::string> allLogs;
// FROM ToolAIO.cpp

namespace EngineCore
{
    // FROM EngineCore.cpp
    extern GLFWwindow* mainWindow;
    extern GLManager glManager;

    extern std::vector<std::shared_ptr<Actor>> actorsInScene;

    extern WorldSetting worldSettings;
    extern std::vector<ClassItem> classItems;
    extern std::vector<ActorItem> actorItems;
    // FROM EngineCore.cpp

    void RenderEditor()
    {
        // Render Editor UI
        // There Are 5 Parts of the Editor
        // Part 1 - Left Panel
        // Part 2 - Right Panel
        // Part 3 - Tool Box
        // Part 4 - Scene
        // Part 5 - Log

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Mouse Event
        if (Event::mouseAsCursor)
        {
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }

        int displayW, displayH;
        glfwGetFramebufferSize(mainWindow, &displayW, &displayH);

        // Set Style
        ImGuiStyle& Style = ImGui::GetStyle();
        Style.FrameRounding = 1;
        Style.FrameBorderSize = 1;
        Style.WindowRounding = 3;

        // === CONST ===
        // Global Const
        const float logHeight = 36;
        const float leftWindowWidth = 256;
        const float rightWindowWidth = 256;
        const float toolBoxHeight = 90;
        const float assetHeight = 172;
        const float actorListHeight = displayH - assetHeight - logHeight;
        const float detailHeight = displayH >> 1;
        const float infoHeight = displayH - detailHeight - logHeight;
        const float centerWidth = displayW - leftWindowWidth - rightWindowWidth;
        const float centerHeight = displayH - toolBoxHeight - logHeight;

        const auto mainWinFlag =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        const auto popWinFlag =
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse;

        const ImVec4 textColor = ImVec4(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);

        // Part 2 Const
        const double lightPowerMin = 0.1;
        const double lightPowerMax = 20.0;

        const char* skyboxChoices = "Creek\0Water\0";
        const int   skyboxChoicesCount = 2;

        // Part3 Const
        const ImVec2 toolboxButtonSize = ImVec2(72, 32);

        // === CONST ===

        // === STATIC ===
        // Global Static
        static std::vector<std::string> assetList;
        static std::vector<std::string> assetTypeList;
        
        auto localUpdateAssetList = []
        (
            std::vector<std::string>& assetList,
            std::vector<std::string>& assetTypeList
        )
        {
            assetList.clear();
            assetTypeList.clear();

            for (const auto& singleFile : std::filesystem::directory_iterator(G_IMPORT_PATH))
            {
                std::string fileName = singleFile.path().filename().u8string();
                std::string upperName = boost::to_upper_copy<std::string>(fileName);

                bool rightType = true;
                if (endsWith(upperName, "OBJ"))
                {
                    assetTypeList.push_back("OBJ");
                }
                else if (endsWith(upperName, "PNG"))
                {
                    assetTypeList.push_back("PNG");
                }
                else rightType = false;

                if (rightType)
                {
                    assetList.push_back(fileName);
                }
            }
        };
#define REENGINE_UPDATE_ASSET_LIST localUpdateAssetList(assetList,assetTypeList)

        // Part 1 Static
        static uint8_t leftEnableTab = 0;// Actor List or Class List

        static int actorCurrent = 0;
        static int classCurrent = 0;

        // Part 2 Static
        static int tagCurrent = -1;
        static std::string tagCurrentStr;

        static float  lightDirf3[3];
        static float  lightColorf3[3];
        static double lightPower = 0;

        static int skyboxCurrent = 0;

        // Part 3 Static
        static char newClassName[16];
        static bool renderNewClass = false; // Check box
        static std::vector<const char*> renderNewClassModel;
        static int renderNewClassModelCurrent = 0;
        static std::vector<const char*> renderNewClassTexture;
        static int renderNewClassTextureCurrent = 0;
        static float renderNewClassDiffuseColorf3[3];

        static char newActorName[32];
        static int  selectClassInNewActor = 0;// Combo

        static char newTagName[32];

        // Part 5 Static
        static char logBuffer[512];
        static char allLogsBuffer[512 * 32];

        // === STATIC ===

        // === LOCAL ===
        // Local Variable Update Every Frame
        std::unordered_set<std::string> actorNameSet;
        std::vector<const char*> actorItemsChar;
        
        std::unordered_set<std::string> classNameSet;
        std::vector<const char*> classItemsChar;
        // === LOCAL ===

        // === Update ===
        for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
        {
            if (!classNameSet.count(std::string(classIndex->m_Name.c_str())))
            {
                classNameSet.insert(std::string(classIndex->m_Name.c_str()));
                classItemsChar.push_back(classIndex->m_Name.c_str());
            }
        }
        for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
        {
            if (!actorNameSet.count(std::string(actorIndex->m_Name.c_str())))
            {
                actorNameSet.insert(std::string(actorIndex->m_Name.c_str()));
                actorItemsChar.push_back(actorIndex->m_Name.c_str());
            }
        }
        // === Update ===
        
        // === Part 1 ===
        {
            int listBoxHeightCount = ((actorListHeight - 68) / ImGui::GetTextLineHeightWithSpacing());
            ImGui::SetNextWindowSize(ImVec2(leftWindowWidth, actorListHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            if (ImGui::Begin("Actors", nullptr, mainWinFlag | ImGuiWindowFlags_NoScrollbar))
            {
                if (ImGui::BeginTabBar("Actors"))
                {
                    if (ImGui::BeginTabItem("Actor List"))
                    {
                        leftEnableTab = 0;
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::ListBox("", &actorCurrent, actorItemsChar.data(), actorItemsChar.size(), listBoxHeightCount);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Class List"))
                    {
                        leftEnableTab = 1;
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::ListBox("", &classCurrent, classItemsChar.data(), classItemsChar.size(), listBoxHeightCount);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::End();
            }

            ImGui::SetNextWindowSize(ImVec2(leftWindowWidth, assetHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, actorListHeight), ImGuiCond_Always);
            if (ImGui::Begin("Assets", nullptr, mainWinFlag | ImGuiWindowFlags_NoScrollbar))
            {
                if (ImGui::Button("View Assets", ImVec2(leftWindowWidth - 16, 64)))
                {
                    REENGINE_UPDATE_ASSET_LIST;

                    ImGui::OpenPopup("View Assets");
                }

                if (ImGui::BeginPopupModal("View Assets", nullptr, popWinFlag))
                {
                    if (ImGui::BeginTable("Assets List", 2,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg,
                        ImVec2(480, 200)))
                    {
                        
                        ImGui::TableSetupColumn("Type");
                        ImGui::TableSetupColumn("Name");
                        ImGui::TableHeadersRow();

                        for (int i = 0; i < assetList.size(); ++i)
                        {
                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextColored(textColor, "%s", assetTypeList[i].c_str());

                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%s", assetList[i].c_str());
                        }

                        ImGui::EndTable();
                    }

                    ImGui::Separator();
                    if (ImGui::Button("Close"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                if (ImGui::Button("Load Assets", ImVec2(leftWindowWidth - 16, 64)))
                {
                    ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", ".obj,.png", "C:\\");
                }

                // Load Asset
                ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);
                if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey",
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoCollapse))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                        std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

                        Out::Log(pType::MESSAGE, "Loading %s", filePathName.c_str());

                        try
                        {
                            std::filesystem::copy_file(filePathName, std::string(G_IMPORT_PATH) + fileName);
                        }
                        catch (...)
                        {
                            Out::Log(pType::ERROR, "Import Failed");
                        }
                    }
                    ImGuiFileDialog::Instance()->Close();
                }
                ImGui::End();
            }
        }

        // === Part 2 ===
        {
            ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, detailHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(displayW - rightWindowWidth, 0), ImGuiCond_Always);
            if (ImGui::Begin("Details", nullptr, mainWinFlag | ImGuiWindowFlags_AlwaysVerticalScrollbar))
            {
                if (leftEnableTab == 0)
                {
                    ImGui::TextColored(textColor, "=== Actor Details ===");
                    ImGui::Separator();
                    if (actorCurrent >= 0 && actorCurrent < actorItemsChar.size())
                    {
                        ImGui::Text("Actor Name: %s", actorItemsChar[actorCurrent]);
                        ImGui::Text("Actor Type: %s", actorItems[actorCurrent].m_ClassName.c_str());
                        if (ImGui::TreeNode("   Tags:"))
                        {
                            int tagIndex = 0;
                            for (auto i = actorItems[actorCurrent].m_Tags.begin(); i != actorItems[actorCurrent].m_Tags.end(); ++i)
                            {
                                std::string Tag = "> [" + *i + "]";
                                if (ImGui::Selectable(Tag.c_str(), tagIndex == tagCurrent))
                                {
                                    tagCurrent = tagIndex;
                                    tagCurrentStr = *i;
                                }
                                tagIndex += 1;
                            }
                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        ImGui::Text("No Actor Selected");
                    }
                    ImGui::Text("");

                    ImGui::TextColored(textColor, "=== Actor Transform ===");
                    ImGui::Separator();
                    if (actorCurrent >= 0 && actorCurrent < actorItemsChar.size())
                    {
                        
                    }
                    else
                    {
                        ImGui::Text("No Actor Selected");
                    }
                }
                else if (leftEnableTab == 1)
                {
                    ImGui::TextColored(textColor, "=== Class Details ===");
                    ImGui::Separator();
                    if (classCurrent >= 0 && classCurrent < classItemsChar.size())
                    {
                        ImGui::Text("Class Name: %s", classItemsChar[classCurrent]);
                    }
                    else
                    {
                        ImGui::Text("No Class Selected");
                    }
                }
                ImGui::End();
            }

            ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, infoHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(displayW - rightWindowWidth, detailHeight), ImGuiCond_Always);
            if (ImGui::Begin("Infos", nullptr, mainWinFlag | ImGuiWindowFlags_AlwaysVerticalScrollbar))
            {
                ImGui::TextColored(textColor, "=== Camera ===");
                ImGui::Separator();
                auto cameraLocation = Event::getCameraLocation();
                auto cameraDir = Event::getCameraDir();

                if (Event::mouseAsCursor)
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "> Cursor");
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "> Move");
                }

                ImGui::Text("Location (%.2lf,%.2lf,%.2lf)", cameraLocation.x(), cameraLocation.y(), cameraLocation.z());
                ImGui::Text("Forward  (%.2lf,%.2lf,%.2lf)", cameraDir.x(), cameraDir.y(), cameraDir.z());
                ImGui::Text("");

                auto glLightDir = glManager.getLightDir();
                auto glLightColor = glManager.getLightColor();
                auto glLightPower = glManager.getLightPower();

                lightDirf3[0] = glLightDir.x();
                lightDirf3[1] = glLightDir.y();
                lightDirf3[2] = glLightDir.z();

                lightColorf3[0] = glLightColor.x();
                lightColorf3[1] = glLightColor.y();
                lightColorf3[2] = glLightColor.z();

                lightPower = glLightPower;

                ImGui::TextColored(textColor, "=== Light ===");
                ImGui::Separator();

                ImGui::Text("Direction");
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::InputFloat3("", lightDirf3);

                ImGui::Text("Color");
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::ColorEdit3("", lightColorf3);

                ImGui::Text("Power");
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::SliderScalar("", ImGuiDataType_Double, &lightPower, &lightPowerMin, &lightPowerMax);

                glManager.setLightDir(Direction(lightDirf3[0], lightDirf3[1], lightDirf3[2]));
                glManager.setLightColor(Color(lightColorf3[0], lightColorf3[1], lightColorf3[2]));
                glManager.setLightPower(lightPower);
                ImGui::Text("");

                ImGui::TextColored(textColor, "=== Skybox ===");
                ImGui::Separator();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::Combo(" ", &skyboxCurrent, skyboxChoices, skyboxChoicesCount);

                ImGui::End();
            }
        }

        // === Part 3 ===
        {
            ImGui::SetNextWindowSize(ImVec2(centerWidth, toolBoxHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(leftWindowWidth, 0), ImGuiCond_Always);
            if (ImGui::Begin("Tools", nullptr, mainWinFlag | ImGuiWindowFlags_NoScrollbar))
            {
                if (ImGui::BeginTabBar("Tools"))
                {
                    if (ImGui::BeginTabItem("File"))
                    {
                        if (ImGui::Button("Read", toolboxButtonSize))
                        {
                            readProject(worldSettings, classItems, actorItems);
                            applyWorldSettings();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Save", toolboxButtonSize))
                        {
                            collectWorldSettings();
                            saveProject(worldSettings, classItems, actorItems);
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Class"))
                    {
                        if (ImGui::Button("New Class", toolboxButtonSize))
                        {
                            REENGINE_UPDATE_ASSET_LIST;

                            renderNewClassModel.clear();
                            renderNewClassTexture.clear();

                            for (int i = 0; i < assetList.size(); ++i)
                            {
                                if (assetTypeList[i] == "OBJ")
                                {
                                    renderNewClassModel.push_back(assetList[i].c_str());
                                }
                                else if (assetTypeList[i] == "PNG")
                                {
                                    renderNewClassTexture.push_back(assetList[i].c_str());
                                }
                            }

                            renderNewClassModelCurrent = 0;
                            renderNewClassTextureCurrent = 0;

                            ImGui::OpenPopup("New Class");
                        }
                        if (ImGui::BeginPopupModal("New Class", nullptr, popWinFlag))
                        {
                            ImGui::Text("Guide for Create new Class");
                            ImGui::Separator();
                            ImGui::Text("Input the Class Name");
                            ImGui::InputText("", newClassName, IM_ARRAYSIZE(newClassName));
                            ImGui::Checkbox("Render", &renderNewClass);

                            if (renderNewClass)
                            {
                                ImGui::Text("Select Model");
                                ImGui::Combo("[M]", &renderNewClassModelCurrent,
                                    renderNewClassModel.data(), renderNewClassModel.size());

                                ImGui::Text("Select Diffuse Texture");
                                ImGui::Combo("[T]", &renderNewClassTextureCurrent,
                                    renderNewClassTexture.data(), renderNewClassTexture.size());

                                ImGui::Text("Select Diffuse Color");
                                ImGui::ColorEdit3("", renderNewClassDiffuseColorf3);
                            }

                            ImGui::Separator();

                            if (ImGui::Button("Add"))
                            {
                                if (checkSimpleStr(newClassName) && !classNameSet.count(newClassName))
                                {
                                    bool canAddClass = false;
                                    if (renderNewClass)
                                    {
                                        if (renderNewClassModelCurrent >= 0 &&
                                            renderNewClassModelCurrent < renderNewClassModel.size() &&
                                            renderNewClassTextureCurrent >= 0 &&
                                            renderNewClassTextureCurrent < renderNewClassTexture.size())
                                        {
                                            canAddClass = true;
                                        }
                                    }
                                    else canAddClass = true;

                                    if (canAddClass)
                                    {
                                        addClassToProject(newClassName);

                                        ClassItem newClass;
                                        newClass.m_Name = std::string(newClassName);
                                        newClass.Render = renderNewClass;
                                        if (renderNewClass)
                                        {
                                            newClass.m_ModelFile = renderNewClassModel[renderNewClassModelCurrent];
                                            newClass.m_DiffuseTextureFile = renderNewClassTexture[renderNewClassTextureCurrent];

                                            newClass.m_DiffuseColor.x() = renderNewClassDiffuseColorf3[0];
                                            newClass.m_DiffuseColor.y() = renderNewClassDiffuseColorf3[1];
                                            newClass.m_DiffuseColor.z() = renderNewClassDiffuseColorf3[2];
                                        }

                                        classItems.push_back(newClass);

                                        collectWorldSettings();
                                        saveProject(worldSettings, classItems, actorItems);

                                        *newClassName = 0;
                                        renderNewClass = false;

                                        ImGui::CloseCurrentPopup();
                                    }
                                }
                                else
                                {
                                    Out::Log(pType::ERROR, "Wrong Class Name");
                                }
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Cancel"))
                            {
                                *newClassName = 0;
                                renderNewClass = false;

                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Actor"))
                    {
                        if (ImGui::Button("New Actor", toolboxButtonSize))
                        {
                            if (!classItemsChar.empty())
                            {
                                ImGui::OpenPopup("New Actor");
                            }
                            else
                            {
                                Out::Log(pType::ERROR, "Please Add a Class First");
                            }
                        }
                        if (ImGui::BeginPopupModal("New Actor", nullptr, popWinFlag))
                        {
                            ImGui::Text("Guide for Create new Actor");
                            ImGui::Separator();

                            ImGui::Text("Input the Actor Name");
                            ImGui::InputText("", newActorName, IM_ARRAYSIZE(newActorName));
                            ImGui::Text("Selete the Class");
                            ImGui::Combo(" ", &selectClassInNewActor, classItemsChar.data(), classItemsChar.size());

                            ImGui::Separator();

                            if (ImGui::Button("Add"))
                            {
                                if (checkSimpleStr(newActorName) &&
                                    !actorNameSet.count(newActorName) &&
                                    selectClassInNewActor >= 0 &&
                                    selectClassInNewActor < classItemsChar.size())
                                {
                                    ActorItem newActor;
                                    newActor.m_Name = std::string(newActorName);
                                    newActor.m_ClassName = classItems[selectClassInNewActor].m_Name;
                                    newActor.m_Tags.insert(newActor.m_ClassName);

                                    newActor.m_Location = Eigen::Vector3d(0, 0, 0);
                                    newActor.m_Rotation = Eigen::Vector3d(0, 0, 0);
                                    newActor.m_Scale    = Eigen::Vector3d(1, 1, 1);

                                    actorItems.push_back(newActor);

                                    *newActorName = 0;
                                    ImGui::CloseCurrentPopup();
                                }
                                else
                                {
                                    Out::Log(pType::ERROR, "Wrong Actor Name");
                                }
                            }
                            
                            ImGui::SameLine();
                            
                            if (ImGui::Button("Cancel"))
                            {
                                *newActorName = 0;
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::EndPopup();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Del Actor", toolboxButtonSize))
                        {
                            if (leftEnableTab == 0 && actorCurrent >= 0 && actorCurrent < actorItemsChar.size())
                            {
                                Out::Log(pType::MESSAGE, "Delete Actor : %s", actorItemsChar[actorCurrent]);
                                actorItems.erase(actorItems.begin() + actorCurrent);
                            }
                            else
                            {
                                Out::Log(pType::ERROR, "No Selected Actor");
                            }
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("New Tag", toolboxButtonSize))
                        {
                            if (leftEnableTab == 0 && actorCurrent >= 0 && actorCurrent < actorItemsChar.size())
                            {
                                ImGui::OpenPopup("New Tag");
                            }
                        }
                        if (ImGui::BeginPopupModal("New Tag", nullptr, popWinFlag))
                        {
                            ImGui::Text("Guide for Add a Tag for %s", actorItemsChar[actorCurrent]);
                            ImGui::Separator();
                            ImGui::Text("Input the Tag Name");
                            ImGui::InputText("", newTagName, IM_ARRAYSIZE(newTagName));

                            ImGui::Separator();

                            if (ImGui::Button("Add"))
                            {
                                if (checkSimpleStr(newTagName))
                                {
                                    actorItems[actorCurrent].m_Tags.insert(newTagName);

                                    *newTagName = 0;
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Cancel"))
                            {
                                *newTagName = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Del Tag", toolboxButtonSize))
                        {
                            if (leftEnableTab == 0 &&
                                actorCurrent >= 0 && actorCurrent < actorItemsChar.size() &&
                                tagCurrent >= 0 && tagCurrent < actorItems[actorCurrent].m_Tags.size())
                            {
                                Out::Log(pType::MESSAGE, "Delete Tag [%s] for %s", tagCurrentStr.c_str(),
                                    actorItemsChar[actorCurrent]);
                                actorItems[actorCurrent].m_Tags.erase(tagCurrentStr);
                            }
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Help"))
                    {
                        if (ImGui::Button("About", toolboxButtonSize))
                        {
                            ImGui::OpenPopup("About");
                        }
                        if (ImGui::BeginPopupModal("About", nullptr, popWinFlag))
                        {
                            ImGui::Text("About reEngine");
                            ImGui::Separator();
                            ImGui::Text("reEngine is an Open Source Game Engine");
                            ImGui::Text("https://github.com/GZhonghui/reEngine");
                            ImGui::Text("");
                            ImGui::Separator();

                            if (ImGui::Button("Close"))
                            {
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::End();
            }
        }

        // === Part 4 ===
        {
            ImGui::SetNextWindowSize(ImVec2(centerWidth, centerHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(leftWindowWidth, toolBoxHeight), ImGuiCond_Always);
            if (ImGui::Begin("Scene", nullptr, mainWinFlag | ImGuiWindowFlags_NoScrollbar))
            {
                auto sceneWindowSize = ImGui::GetWindowSize();
                sceneWindowSize.x -= 16;
                sceneWindowSize.y -= 36;

                auto sceneTextureID = RenderEditorScene(sceneWindowSize.x, sceneWindowSize.y);
                ImGui::Image((void*)sceneTextureID, sceneWindowSize, ImVec2(0, 1), ImVec2(1, 0));

                ImGui::End();
            }
        }

        // === Part 5 ===
        {
            ImGui::SetNextWindowSize(ImVec2(displayW, logHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, displayH - logHeight), ImGuiCond_Always);
            if (ImGui::Begin("Logs", nullptr, mainWinFlag | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
            {
                if (!allLogs.empty())
                {
                    strcpy(logBuffer, allLogs.back().c_str());
                }
                else
                {
                    strcpy(logBuffer, "Welcome to reEngine!");
                }
                ImGui::PushItemWidth(-160);
                ImGui::InputText("", logBuffer, sizeof(logBuffer), ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("All Logs"))
                {
                    ImGui::OpenPopup("All Logs");

                    std::string allLogsInOne;
                    for (auto i = allLogs.begin(); i != allLogs.end(); ++i)
                    {
                        allLogsInOne += *i + "\n";
                    }
                    if (allLogsInOne.empty())
                    {
                        allLogsInOne = "Clear";
                    }

                    strcpy(allLogsBuffer, allLogsInOne.c_str());
                }
                if (ImGui::BeginPopupModal("All Logs", nullptr, popWinFlag))
                {
                    ImGui::InputTextMultiline("", allLogsBuffer, sizeof(allLogsBuffer), ImVec2(480, 200), ImGuiInputTextFlags_ReadOnly);
                    ImGui::Separator();
                    if (ImGui::Button("Close"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear Logs"))
                {
                    allLogs.clear();
                }

                ImGui::End();
            }
        }
        
        ImGui::Render();

        glViewport(0, 0, displayW, displayH);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
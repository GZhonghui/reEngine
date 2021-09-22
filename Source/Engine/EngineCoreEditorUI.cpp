#include"EngineCore.h"

namespace EngineCore
{
    // FROM EngineCore.cpp
    extern GLFWwindow* mainWindow;
    extern GLManager glManager;

    // Game Mode Only
    extern std::vector<std::shared_ptr<Actor>> actorsInSceneOfGame;

    extern std::unordered_map<std::string, std::shared_ptr<GLRenderable>> classesInSceneForRender;

    extern WorldSetting worldSettings;
    extern std::vector<ClassItem> classItems;
    extern std::vector<ActorItem> actorItems;
    extern std::vector<ComponentItem> componentItems;

    extern int  coreSelectedActorInEditorScene;
    extern bool coreRenderGridInEditorScene;
    extern int  coreRenderModeInEditorScene;
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
        if (Event::mouseAsCursor) ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        else                      ImGui::GetIO().ConfigFlags |=  ImGuiConfigFlags_NoMouse;

        int displayW, displayH;
        glfwGetFramebufferSize(mainWindow, &displayW, &displayH);

        // Set Style
        ImGuiStyle& Style = ImGui::GetStyle();
        Style.FrameRounding   = 1;
        Style.FrameBorderSize = 1;
        Style.WindowRounding  = 3;

        // === CONST ===
        // Global Const
        const float logHeight        = 36;
        const float leftWindowWidth  = 256;
        const float rightWindowWidth = 256;
        const float toolBoxHeight    = 90;
        const float assetHeight      = 172;
        const float actorListHeight  = displayH - assetHeight - logHeight;
        const float detailHeight     = displayH >> 1;
        const float infoHeight       = displayH - detailHeight - logHeight;
        const float centerWidth      = displayW - leftWindowWidth - rightWindowWidth;
        const float centerHeight     = displayH - toolBoxHeight - logHeight;

        const auto mainWinFlag =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        const auto popWinFlag =
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse;

        const ImVec4 textColorGreen = ImVec4(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);

        const char* avaliableShader      = glManager.getSupportShaderList();
        const int   avaliableShaderCount = glManager.getSupportShaderCount();

        // Part 2 Const
        const double lightPowerMin = 0.1;
        const double lightPowerMax = 20.0;

        const char* skyboxChoices      = glManager.getSkyboxList();
        const int   skyboxChoicesCount = glManager.getSkyboxCount();

        const double cameraMoveSpeedMin = 1.0;
        const double cameraMoveSpeedMax = 20.0;

        // Part3 Const
        const ImVec2 toolboxButtonSize = ImVec2(108, 32);
        // === CONST ===

        // === STATIC ===
        // Global Static
        static std::vector<std::string> assetList;
        static std::vector<aType>       assetTypeList;

        // Part 1 Static
        static uint8_t leftEnableTab = 0;// Actor List or Class List or Component

        static int actorCurrent = -1;
        static int classCurrent = -1;
        static int componentCurrent = -1;

        static int selectedAssetID = -1;// In Asset Viewer

        // Part 2 Static
        static int tagOfActorCurrent = -1;
        static std::string tagOfActorCurrentStr;

        static char newTagName[32];

        static float actorLocationf3[3];
        static float actorRotationf3[3];
        static float actorScalef3[3];

        static int componentOfActorCurrent = -1;
        static std::string componentOfActorCurrentStr;

        static int actorAddComponentCurrent = 0;

        static int classShaderCurrent = 0;

        static int classModelCurrent = 0;

        static float classN = 1.4;

        static ImVec4 classDiffuseColor;

        static bool classEnableDiffuse  = false;
        static int  classDiffuseCurrent = 0;

        static bool classEnableNormal  = false;
        static int  classNormalCurrent = 0;

        static bool classEnableSpecular  = false;
        static int  classSpecularCurrent = 0;

        static float cameraMoveSpeed = 10.0;
        static float cameraFOV = 60;

        static float lightDirf3[3];
        static float lightColorf3[3];
        static float lightPower = 1.0;

        static int  skyboxCurrent = 0;
        static bool renderGrid = true;
        static int  renderMode = 0; // 0 : Fill; 1: Line

        // Part 3 Static
        static char newActorName[32];
        static int  selectClassInNewActor = 0; // Combo

        static char newClassName[32];
        static bool newClassAddActor = false; // Check box
        static bool renderNewClass = false; // Check box

        static int renderNewClassModelCurrent = 0;

        static int renderNewClassShaderCurrent = 0;

        static float renderNewClassDiffuseColorf3[3];

        static bool renderNewClassEnableDiffuseTexture  = false;
        static int  renderNewClassDiffuseTextureCurrent = 0;

        static bool renderNewClassEnableNormalTexture  = false;
        static int  renderNewClassNormalTextureCurrent = 0;

        static bool renderNewClassEnableSpecularTexture  = false;
        static int  renderNewClassSpecularTextureCurrent = 0;

        static float renderNewClassN = 1.4;

        static char newComponentName[32];

        // Part 5 Static
        static char logBuffer[512];
        static char allLogsBuffer[512 * 32];
        // === STATIC ===

        // === Exchange With Engine Core
        coreSelectedActorInEditorScene = actorCurrent;
        coreRenderGridInEditorScene    = renderGrid;
        coreRenderModeInEditorScene    = renderMode;
        // === Exchange With Engine Core

        // === Exchange Whit Others
        Event::cameraMoveSpeed = cameraMoveSpeed;

        Event::mainCamera.setFOV(cameraFOV);
        // === Exchange Whit Others

        // === LOCAL ===
        // Local Variable Update Every Frame
        std::unordered_set<std::string> actorNameSet;
        std::vector<const char*> actorItemsChar;
        
        std::unordered_set<std::string> classNameSet;
        std::vector<const char*> classItemsChar;

        std::unordered_set<std::string> componentNameSet;
        std::vector<const char*> componentItemsChar;

        std::vector<const char*> assetModel;
        std::vector<const char*> assetTexture;

        std::unordered_map<std::string, int> revAssetModel;
        std::unordered_map<std::string, int> revAssetTexture;
        
        std::unordered_set<std::string> cantDeleteAssets;
        // === LOCAL ===

        // Update Once
        static bool FirstFrame = true;
        if (FirstFrame)
        {
            FirstFrame = false;
            
            AssetManager::getAssetList(assetList, assetTypeList);
        }
        // Update Once

        // === Update ===
        for (auto actorIndex = actorItems.begin(); actorIndex != actorItems.end(); ++actorIndex)
        {
            if (!actorNameSet.count(actorIndex->m_Name))
            {
                actorNameSet.insert(actorIndex->m_Name);
                actorItemsChar.push_back(actorIndex->m_Name.c_str());
            }
        }
        for (auto classIndex = classItems.begin(); classIndex != classItems.end(); ++classIndex)
        {
            if (!classNameSet.count(classIndex->m_Name))
            {
                classNameSet.insert(classIndex->m_Name);
                classItemsChar.push_back(classIndex->m_Name.c_str());
            }

            if (classIndex->m_Render)
            {
                if (!classesInSceneForRender.count(classIndex->m_Name))
                {
                    classesInSceneForRender[classIndex->m_Name] = std::make_shared<GLRenderable>();

                    std::shared_ptr<GLRenderable> thisRenderObj = classesInSceneForRender[classIndex->m_Name];
                    
                    thisRenderObj->setShader(classIndex->m_Shader);

                    thisRenderObj->reLoadModel(classIndex->m_Model);

                    thisRenderObj->setDiffuseColor(classIndex->m_DiffuseColor);

                    if (classIndex->m_EnableDiffuseTexture)
                    {
                        thisRenderObj->EnableDiffuseTexture(true);
                        thisRenderObj->reLoadDiffuseTexture(classIndex->m_DiffuseTexture);
                    }
                    if (classIndex->m_EnableNormalTexture)
                    {
                        thisRenderObj->EnableNormalTexture(true);
                        thisRenderObj->reLoadNormalTexture(classIndex->m_NormalTexture);
                    }
                    if (classIndex->m_EnableSpecularTexture)
                    {
                        thisRenderObj->EnableSpecularTexture(true);
                        thisRenderObj->reLoadSpecularTexture(classIndex->m_SpecularTexture);
                    }

                    thisRenderObj->setN(classIndex->m_N);
                }
                
                if (!classIndex->m_Model.empty())
                {
                    cantDeleteAssets.insert(classIndex->m_Model);
                }
                if (classIndex->m_EnableDiffuseTexture && !classIndex->m_DiffuseTexture.empty())
                {
                    cantDeleteAssets.insert(classIndex->m_DiffuseTexture);
                }
                if (classIndex->m_EnableNormalTexture && !classIndex->m_NormalTexture.empty())
                {
                    cantDeleteAssets.insert(classIndex->m_NormalTexture);
                }
                if (classIndex->m_EnableSpecularTexture && !classIndex->m_SpecularTexture.empty())
                {
                    cantDeleteAssets.insert(classIndex->m_SpecularTexture);
                }
            }
            else
            {
                classesInSceneForRender[classIndex->m_Name] = nullptr;
            }
        }
        for (auto componentIndex = componentItems.begin(); componentIndex != componentItems.end(); ++componentIndex)
        {
            if (!componentNameSet.count(componentIndex->m_Name))
            {
                componentNameSet.insert(componentIndex->m_Name);
                componentItemsChar.push_back(componentIndex->m_Name.c_str());
            }
        }

        int assetModelIndex = 0;
        int assetTextureIndex = 0;

        for (auto assetIdx = 0; assetIdx < assetList.size(); ++assetIdx)
        {
            if (assetTypeList[assetIdx] == aType::MODEL)
            {
                assetModel.push_back(assetList[assetIdx].c_str());
                revAssetModel[assetList[assetIdx]] = assetModelIndex++;
            }
            else if (assetTypeList[assetIdx] == aType::TEXTURE)
            {
                assetTexture.push_back(assetList[assetIdx].c_str());
                revAssetTexture[assetList[assetIdx]] = assetTextureIndex++;
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
                if (ImGui::BeginTabBar("Lists"))
                {
                    if (ImGui::BeginTabItem("Actor"))
                    {
                        leftEnableTab = 0;
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::ListBox("", &actorCurrent,
                            actorItemsChar.data(), actorItemsChar.size(), listBoxHeightCount);
                        ImGui::PopItemWidth();
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Class"))
                    {
                        leftEnableTab = 1;
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::ListBox("", &classCurrent,
                            classItemsChar.data(), classItemsChar.size(), listBoxHeightCount);
                        ImGui::PopItemWidth();
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Component"))
                    {
                        leftEnableTab = 2;
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::ListBox("", &componentCurrent,
                            componentItemsChar.data(), componentItemsChar.size(), listBoxHeightCount);
                        ImGui::PopItemWidth();
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
                    AssetManager::getAssetList(assetList, assetTypeList);

                    ImGui::OpenPopup("View Assets");
                }

                if (ImGui::BeginPopupModal("View Assets", nullptr, popWinFlag))
                {
                    if (ImGui::BeginTable("Assets List", 4,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg,
                        ImVec2(480, 200)))
                    {
                        ImGui::TableSetupColumn("ID");
                        ImGui::TableSetupColumn("Use");
                        ImGui::TableSetupColumn("Type");
                        ImGui::TableSetupColumn("Name");
                        ImGui::TableHeadersRow();

                        for (int i = 0; i < assetList.size(); ++i)
                        {
                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%03d", i);

                            ImGui::TableSetColumnIndex(1);
                            if (cantDeleteAssets.count(assetList[i]))
                            {
                                ImGui::Text("Using");
                            }
                            else
                            {
                                ImGui::Text("Blank");
                            }

                            ImGui::TableSetColumnIndex(2);
                            std::string assetTypeStr;
                            if (assetTypeList[i] == aType::MODEL)
                            {
                                assetTypeStr = " Model ";
                            }
                            else if (assetTypeList[i] == aType::TEXTURE)
                            {
                                assetTypeStr = "Texture";
                            }
                            ImGui::TextColored(textColorGreen, "%s", assetTypeStr.c_str());

                            ImGui::TableSetColumnIndex(3);
                            if (ImGui::Selectable(assetList[i].c_str(), selectedAssetID == i,
                                ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups))
                            {
                                selectedAssetID = i;
                            }
                        }
                        ImGui::EndTable();
                    }

                    ImGui::Separator();
                    if (ImGui::Button("Close"))
                    {
                        selectedAssetID = -1;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Refresh"))
                    {
                        AssetManager::getAssetList(assetList, assetTypeList);
                    }
                    ImGui::SameLine();

                    bool canDeleteAsset = Inside(selectedAssetID, 0, assetList.size() - 1) &&
                        !cantDeleteAssets.count(assetList[selectedAssetID]);

                    if (!canDeleteAsset) uiBeginDisable();
                    if (ImGui::Button("Delete"))
                    {
                        AssetManager::deleteAsset(assetList[selectedAssetID]);
                        AssetManager::getAssetList(assetList, assetTypeList);
                    }
                    if (!canDeleteAsset) uiEndDisable();

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
                        std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

                        AssetManager::addAsset(filePathName, fileName);
                        AssetManager::getAssetList(assetList, assetTypeList);
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
                    ActorItem* pSelectedActor = Inside(actorCurrent, 0, actorItems.size() - 1) ?
                        &actorItems[actorCurrent] : nullptr;

                    ImGui::TextColored(textColorGreen, "=== Actor Details ===");
                    ImGui::Separator();

                    if (pSelectedActor)
                    {
                        ImGui::Text("Actor Name: %s", pSelectedActor->m_Name.c_str());
                        ImGui::Text("Actor Type: %s", pSelectedActor->m_ClassName.c_str());
                        if (ImGui::TreeNode("   Tags:"))
                        {
                            int tagIndex = 0;
                            for (auto i = pSelectedActor->m_Tags.begin(); i != pSelectedActor->m_Tags.end(); ++i)
                            {
                                std::string Tag = "> [" + *i + "]";
                                if (ImGui::Selectable(Tag.c_str(), tagIndex == tagOfActorCurrent))
                                {
                                    tagOfActorCurrent = tagIndex;
                                    tagOfActorCurrentStr = *i;
                                }
                                tagIndex += 1;
                            }

                            if (ImGui::Button("New Tag"))
                            {
                                ImGui::OpenPopup("New Tag");
                            }
                            if (ImGui::BeginPopup("New Tag", popWinFlag))
                            {
                                ImGui::Text("Add a Tag for [%s]", pSelectedActor->m_Name.c_str());
                                ImGui::Separator();
                                ImGui::Text("Input the Tag Name");
                                ImGui::InputText("", newTagName, IM_ARRAYSIZE(newTagName));
                                ImGui::Separator();

                                bool canAddTag = checkSimpleStr(newTagName) && !pSelectedActor->m_Tags.count(newTagName);

                                if (!canAddTag) uiBeginDisable();

                                if (ImGui::Button("Add"))
                                {
                                    pSelectedActor->m_Tags.insert(newTagName);

                                    *newTagName = 0;
                                    ImGui::CloseCurrentPopup();
                                }

                                if (!canAddTag) uiEndDisable();

                                ImGui::SameLine();
                                if (ImGui::Button("Cancel"))
                                {
                                    *newTagName = 0;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            ImGui::SameLine();
                            
                            bool canDeleteTag = Inside(tagOfActorCurrent, 0, pSelectedActor->m_Tags.size() - 1);
                            if (!canDeleteTag) uiBeginDisable();
                            if (ImGui::Button("Del Tag"))
                            {
                                Out::Log(pType::MESSAGE, "Delete Tag [%s] for [%s]", tagOfActorCurrentStr.c_str(),
                                    pSelectedActor->m_Name.c_str());
                                actorItems[actorCurrent].m_Tags.erase(tagOfActorCurrentStr);
                            }
                            if (!canDeleteTag) uiEndDisable();
                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        tagOfActorCurrent = -1;
                        tagOfActorCurrentStr.clear();

                        ImGui::Text("No Actor Selected");
                    }
                    ImGui::Text("");

                    ImGui::TextColored(textColorGreen, "=== Actor Transform ===");
                    ImGui::Separator();
                    if (pSelectedActor)
                    {
                        ImGui::Text("Location ");
                        ImGui::SameLine();
                        if (ImGui::Button("Reset##Location"))
                        {
                            pSelectedActor->m_Location = Eigen::Vector3d(0, 0, 0);
                        }
                        Fill(actorLocationf3, pSelectedActor->m_Location);
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::DragFloat3("##Location", actorLocationf3, 0.01f);
                        ImGui::PopItemWidth();
                        Fill(pSelectedActor->m_Location, actorLocationf3);

                        ImGui::Text("Rotation ");
                        ImGui::SameLine();
                        if (ImGui::Button("Reset##Rotation"))
                        {
                            pSelectedActor->m_Rotation = Eigen::Vector3d(0, 0, 0);
                        }
                        Fill(actorRotationf3, pSelectedActor->m_Rotation);
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::DragFloat3("##Rotation", actorRotationf3, 0.1f, -360.0f, 360.0f);
                        ImGui::PopItemWidth();
                        Fill(pSelectedActor->m_Rotation, actorRotationf3);

                        ImGui::Text("Scale    ");
                        ImGui::SameLine();
                        if (ImGui::Button("Reset##Scale"))
                        {
                            pSelectedActor->m_Scale = Eigen::Vector3d(1, 1, 1);
                        }
                        Fill(actorScalef3, pSelectedActor->m_Scale);
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::DragFloat3("##Scale", actorScalef3, 0.001f, 0.1f, 10.0f);
                        ImGui::PopItemWidth();
                        Fill(pSelectedActor->m_Scale, actorScalef3);
                    }
                    else
                    {
                        ImGui::Text("N/A");
                    }
                    ImGui::Text("");

                    ImGui::TextColored(textColorGreen, "=== Actor Components ===");
                    ImGui::Separator();
                    if (pSelectedActor)
                    {
                        int componentIndex = 0;
                        for (auto i = pSelectedActor->m_Components.begin(); i != pSelectedActor->m_Components.end(); ++i)
                        {
                            std::string Component = "> " + *i + "";
                            if (ImGui::Selectable(Component.c_str(), componentIndex == componentOfActorCurrent))
                            {
                                componentOfActorCurrent = componentIndex;
                                componentOfActorCurrentStr = *i;
                            }
                            componentIndex += 1;
                        }

                        bool canAddComponent = !componentItems.empty();
                        if (!canAddComponent) uiBeginDisable();
                        if (ImGui::Button("Add Component"))
                        {
                            ImGui::OpenPopup("Add Component");
                        }
                        if (!canAddComponent) uiEndDisable();
                        if (ImGui::BeginPopup("Add Component", popWinFlag))
                        {
                            ImGui::Text("Add Compnent For [%s]", pSelectedActor->m_Name);
                            ImGui::Separator();
                            ImGui::Text("Select Component");
                            ImGui::Combo("##SelectComponent", &actorAddComponentCurrent,
                                componentItemsChar.data(), componentItems.size());
                            ImGui::Separator();

                            bool canClickAdd = Inside(actorAddComponentCurrent, 0, componentItems.size() - 1) &&
                                !pSelectedActor->m_Components.count(componentItems[actorAddComponentCurrent].m_Name);
                            if (!canClickAdd) uiBeginDisable();
                            if (ImGui::Button("Add"))
                            {
                                pSelectedActor->m_Components.insert(componentItems[actorAddComponentCurrent].m_Name);

                                actorAddComponentCurrent = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            if (!canClickAdd) uiEndDisable();

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel"))
                            {
                                actorAddComponentCurrent = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }

                        ImGui::SameLine();

                        bool canDeleteComponent = Inside(componentOfActorCurrent, 0, pSelectedActor->m_Components.size() - 1);
                        if (!canDeleteComponent) uiBeginDisable();
                        if (ImGui::Button("Del Component"))
                        {
                            pSelectedActor->m_Components.erase(componentOfActorCurrentStr);
                        }
                        if (!canDeleteComponent) uiEndDisable();
                    }
                    else
                    {
                        ImGui::Text("N/A");
                    }
                }
                else if (leftEnableTab == 1)
                {
                    ClassItem* selectedClass = Inside(classCurrent, 0, classItems.size() - 1) ?
                        &classItems[classCurrent] : nullptr;
                    std::shared_ptr<GLRenderable> selectedClassRenderObj = selectedClass ?
                        classesInSceneForRender[selectedClass->m_Name] : nullptr;

                    ImGui::TextColored(textColorGreen, "=== Class Details ===");
                    ImGui::Separator();
                    
                    if (selectedClass)
                    {
                        ImGui::Text("Class Name: %s", selectedClass->m_Name);
                    }
                    else
                    {
                        ImGui::Text("No Class Selected");
                    }
                    ImGui::Text("");

                    ImGui::TextColored(textColorGreen, "=== Class Visibility===");
                    ImGui::Separator();
                    if (selectedClass && selectedClass->m_Render)
                    {
                        ImGui::PushItemWidth(-FLT_MIN);

                        ImGui::Text("Model");

                        classModelCurrent = revAssetModel.count(selectedClass->m_Model) ?
                            revAssetModel[selectedClass->m_Model] : 0;
                        ImGui::Combo("##Model", &classModelCurrent, assetModel.data(), assetModel.size());
                        selectedClass->m_Model = assetModel[classModelCurrent];
                        selectedClassRenderObj->reLoadModel(selectedClass->m_Model);

                        ImGui::Text("Shader");

                        classShaderCurrent = glManager.getIdxOfSupportShader(selectedClass->m_Shader);
                        ImGui::Combo("##Shader", &classShaderCurrent, avaliableShader, avaliableShaderCount);
                        selectedClass->m_Shader = glManager.getSupportShaderAt(classShaderCurrent);
                        selectedClassRenderObj->setShader(selectedClass->m_Shader);

                        if (selectedClass->m_Shader == "Default")
                        {
                            ImGui::Text("Diffuse Color");

                            classDiffuseColor.x = selectedClass->m_DiffuseColor.x();
                            classDiffuseColor.y = selectedClass->m_DiffuseColor.y();
                            classDiffuseColor.z = selectedClass->m_DiffuseColor.z();
                            classDiffuseColor.w = 1;

                            const int imageSize = rightWindowWidth - 32;

                            ImGui::ColorEdit3("##DiffuseColor", (float*)&classDiffuseColor);
                            ImGui::ColorButton("", *(ImVec4*)&classDiffuseColor, 0, ImVec2(imageSize, 48));
                            selectedClass->m_DiffuseColor.x() = classDiffuseColor.x;
                            selectedClass->m_DiffuseColor.y() = classDiffuseColor.y;
                            selectedClass->m_DiffuseColor.z() = classDiffuseColor.z;
                            selectedClassRenderObj->setDiffuseColor(selectedClass->m_DiffuseColor);

                            classEnableDiffuse = selectedClass->m_EnableDiffuseTexture;
                            ImGui::Checkbox("Enable Diffuse", &classEnableDiffuse);
                            selectedClass->m_EnableDiffuseTexture = classEnableDiffuse;
                            selectedClassRenderObj->EnableDiffuseTexture(classEnableDiffuse);

                            if (classEnableDiffuse)
                            {
                                classDiffuseCurrent = revAssetTexture.count(selectedClass->m_DiffuseTexture) ?
                                    revAssetTexture[selectedClass->m_DiffuseTexture] : 0;

                                ImGui::Combo("##DiffuseTexture", &classDiffuseCurrent, assetTexture.data(), assetTexture.size());
                                selectedClass->m_DiffuseTexture = assetTexture[classDiffuseCurrent];
                                selectedClassRenderObj->reLoadDiffuseTexture(selectedClass->m_DiffuseTexture);

                                ImGui::Image((void*)selectedClassRenderObj->getDiffuseTextureID(), ImVec2(imageSize, imageSize));
                            }

                            classEnableNormal = selectedClass->m_EnableNormalTexture;
                            ImGui::Checkbox("Enable Normal", &classEnableNormal);
                            selectedClass->m_EnableNormalTexture = classEnableNormal;
                            selectedClassRenderObj->EnableNormalTexture(classEnableNormal);

                            if (classEnableNormal)
                            {
                                classNormalCurrent = revAssetTexture.count(selectedClass->m_NormalTexture) ?
                                    revAssetTexture[selectedClass->m_NormalTexture] : 0;

                                ImGui::Combo("##NormalTexture", &classNormalCurrent, assetTexture.data(), assetTexture.size());
                                selectedClass->m_NormalTexture = assetTexture[classNormalCurrent];
                                selectedClassRenderObj->reLoadNormalTexture(selectedClass->m_NormalTexture);

                                ImGui::Image((void*)selectedClassRenderObj->getNormalTextureID(), ImVec2(imageSize, imageSize));
                            }

                            classEnableSpecular = selectedClass->m_EnableSpecularTexture;
                            ImGui::Checkbox("Enable Specular", &classEnableSpecular);
                            selectedClass->m_EnableSpecularTexture = classEnableSpecular;
                            selectedClassRenderObj->EnableSpecularTexture(classEnableSpecular);

                            if (classEnableSpecular)
                            {
                                classSpecularCurrent = revAssetTexture.count(selectedClass->m_SpecularTexture) ?
                                    revAssetTexture[selectedClass->m_SpecularTexture] : 0;

                                ImGui::Combo("##SpecularTexture", &classSpecularCurrent, assetTexture.data(), assetTexture.size());
                                selectedClass->m_SpecularTexture = assetTexture[classSpecularCurrent];
                                selectedClassRenderObj->reLoadSpecularTexture(selectedClass->m_SpecularTexture);

                                ImGui::Image((void*)selectedClassRenderObj->getSpecularTextureID(), ImVec2(imageSize, imageSize));
                            }
                        }
                        else if (selectedClass->m_Shader == "Glass")
                        {

                        }

                        ImGui::PopItemWidth();
                    }
                    else
                    {
                        ImGui::Text("N/A");
                    }
                }
                else if (leftEnableTab == 2)
                {
                    ImGui::TextColored(textColorGreen, "=== Component Details ===");
                    ImGui::Separator();

                    if (Inside(componentCurrent, 0, componentItems.size() - 1))
                    {
                        ImGui::Text("Component Name: %s", componentItems[componentCurrent].m_Name.c_str());
                    }
                    else
                    {
                        ImGui::Text("No Component Selected");
                    }
                }

                ImGui::End();
            }

            ImGui::SetNextWindowSize(ImVec2(rightWindowWidth, infoHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(displayW - rightWindowWidth, detailHeight), ImGuiCond_Always);
            if (ImGui::Begin("Infos", nullptr, mainWinFlag | ImGuiWindowFlags_AlwaysVerticalScrollbar))
            {
                ImGui::TextColored(textColorGreen, "=== Camera ===");
                ImGui::Separator();
                auto cameraPos = Event::mainCamera.getPosition();
                auto cameraDir = Event::mainCamera.getForward();

                if (Event::mouseAsCursor)
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "> [C]ursor");
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "> [M]oving");
                }

                ImGui::Text("%lf", Event::mainCamera.m_uAngle);
                ImGui::Text("Location (%6.2lf,%6.2lf,%6.2lf)", cameraPos.x(), cameraPos.y(), cameraPos.z());
                ImGui::Text("Forward  (%6.2lf,%6.2lf,%6.2lf)", cameraDir.x(), cameraDir.y(), cameraDir.z());
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::Text("Speed");
                ImGui::SameLine();
                ImGui::SliderFloat("##CameraMoveSpeed", &cameraMoveSpeed, 1.0f, 30.0f);
                ImGui::Text("FOV  ");
                ImGui::SameLine();
                ImGui::SliderFloat("##CameraFOV", &cameraFOV, 45.0f, 75.0f);
                ImGui::PopItemWidth();
                ImGui::Text("");

                ImGui::TextColored(textColorGreen, "=== Light ===");
                ImGui::Separator();

                auto glLightDir   = glManager.getLightDir();
                auto glLightColor = glManager.getLightColor();
                auto glLightPower = glManager.getLightPower();

                Fill(lightDirf3, glLightDir);
                Fill(lightColorf3, glLightColor);

                lightPower = glLightPower;

                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::Text("Direction");
                ImGui::DragFloat3("##LightDirection", lightDirf3, 0.01f, -5.0f, 5.0f);

                ImGui::Text("Color");
                ImGui::ColorEdit3("##LightColor", lightColorf3);

                ImGui::Text("Power");
                ImGui::SliderFloat("##LightPower", &lightPower, 0.1f, 20.0f);
                ImGui::PopItemWidth();

                glManager.setLightDir(Direction(lightDirf3[0], lightDirf3[1], lightDirf3[2]));
                glManager.setLightColor(Color(lightColorf3[0], lightColorf3[1], lightColorf3[2]));
                glManager.setLightPower(lightPower);
                ImGui::Text("");

                ImGui::TextColored(textColorGreen, "=== Skybox ===");
                ImGui::Separator();

                skyboxCurrent = glManager.getNowSkybox();

                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::Combo("##Skybox", &skyboxCurrent, skyboxChoices, skyboxChoicesCount);
                ImGui::PopItemWidth();
                
                glManager.ChangeSkybox(skyboxCurrent);
                ImGui::Text("");

                ImGui::TextColored(textColorGreen, "=== Grid ===");
                ImGui::Separator();
                ImGui::Checkbox("Render Grid", &renderGrid);
                ImGui::Text("");

                ImGui::TextColored(textColorGreen, "=== Mode ===");
                ImGui::Separator();
                if (ImGui::RadioButton("Fill", renderMode == 0))
                {
                    renderMode = 0;
                }
                ImGui::SameLine();
                if (ImGui::RadioButton("Line", renderMode == 1))
                {
                    renderMode = 1;
                }

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
                            readProject(worldSettings, actorItems, classItems, componentItems);
                            applyWorldSettings();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Save", toolboxButtonSize))
                        {
                            collectWorldSettings();
                            saveProject(worldSettings, actorItems, classItems, componentItems);
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Actor"))
                    {
                        bool canAddActorButton = !classItems.empty();
                        if (!canAddActorButton) uiBeginDisable();
                        if (ImGui::Button("New Actor", toolboxButtonSize))
                        {
                            ImGui::OpenPopup("New Actor");
                        }
                        if (!canAddActorButton) uiEndDisable();

                        if (ImGui::BeginPopupModal("New Actor", nullptr, popWinFlag))
                        {
                            ImGui::Text("Guide for Create new Actor");
                            ImGui::Separator();

                            ImGui::Text("Input the Actor Name");
                            ImGui::InputText("", newActorName, IM_ARRAYSIZE(newActorName));
                            ImGui::Text("Selete the Class");
                            ImGui::Combo("##SeletedClass", &selectClassInNewActor, classItemsChar.data(), classItemsChar.size());
                            ImGui::Separator();

                            bool canAddActor = checkSimpleStr(newActorName) &&
                                Inside(selectClassInNewActor, 0, classItems.size() - 1) &&
                                !actorNameSet.count(newActorName);

                            if (!canAddActor) uiBeginDisable();
                            if (ImGui::Button("Add"))
                            {
                                ActorItem newActor;
                                newActor.m_Name = std::string(newActorName);
                                newActor.m_ClassName = classItems[selectClassInNewActor].m_Name;
                                newActor.m_Tags.insert("Actor_" + newActor.m_ClassName);

                                newActor.m_Location = Eigen::Vector3d(0, 0, 0);
                                newActor.m_Rotation = Eigen::Vector3d(0, 0, 0);
                                newActor.m_Scale    = Eigen::Vector3d(1, 1, 1);

                                actorItems.push_back(newActor);

                                *newActorName = 0;
                                selectClassInNewActor = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            if (!canAddActor) uiEndDisable();

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel"))
                            {
                                *newActorName = 0;
                                selectClassInNewActor = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                        ImGui::SameLine();

                        bool canDeleteActor = (leftEnableTab == 0) && Inside(actorCurrent, 0, actorItems.size() - 1);
                        if (!canDeleteActor) uiBeginDisable();
                        if (ImGui::Button("Del Actor", toolboxButtonSize))
                        {
                            Out::Log(pType::MESSAGE, "Delete Actor : %s", actorItemsChar[actorCurrent]);
                            actorItems.erase(actorItems.begin() + actorCurrent);
                        }
                        if (!canDeleteActor) uiEndDisable();

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Class"))
                    {
                        bool canAddClass = (!assetModel.empty()) && (!assetTexture.empty());
                        if (!canAddClass) uiBeginDisable();
                        if (ImGui::Button("New Class", toolboxButtonSize))
                        {
                            AssetManager::getAssetList(assetList, assetTypeList);

                            renderNewClassModelCurrent = 0;
                            renderNewClassShaderCurrent = 0;

                            ImGui::OpenPopup("New Class");
                        }
                        if (!canAddClass) uiEndDisable();

                        if (ImGui::BeginPopupModal("New Class", nullptr, popWinFlag))
                        {
                            ImGui::Text("Guide for Create new Class");
                            ImGui::Separator();
                            ImGui::Text("Input the Class Name");
                            ImGui::InputText("", newClassName, IM_ARRAYSIZE(newClassName));
                            ImGui::Checkbox("Add a Default Actor", &newClassAddActor);
                            ImGui::Separator();
                            ImGui::Checkbox("Render", &renderNewClass);

                            if (renderNewClass)
                            {
                                ImGui::Text("Select Model");
                                ImGui::Combo("##Model", &renderNewClassModelCurrent,
                                    assetModel.data(), assetModel.size());

                                ImGui::Text("Select Shader");
                                ImGui::Combo("##Shader", &renderNewClassShaderCurrent,
                                    avaliableShader, avaliableShaderCount);

                                std::string useShader = glManager.getSupportShaderAt(renderNewClassShaderCurrent);
                                if (useShader == "Default")
                                {
                                    ImGui::Text("Select Diffuse Color");
                                    ImGui::ColorEdit3("##DiffuseColor", renderNewClassDiffuseColorf3);

                                    ImGui::Checkbox("Enable Diffuse Texture", &renderNewClassEnableDiffuseTexture);
                                    if (renderNewClassEnableDiffuseTexture)
                                    {
                                        ImGui::Combo("##DiffuseTexture", &renderNewClassDiffuseTextureCurrent,
                                            assetTexture.data(), assetTexture.size());
                                    }

                                    ImGui::Checkbox("Enable Normal Texture", &renderNewClassEnableNormalTexture);
                                    if (renderNewClassEnableNormalTexture)
                                    {
                                        ImGui::Combo("##NormalTexture", &renderNewClassNormalTextureCurrent,
                                            assetTexture.data(), assetTexture.size());
                                    }

                                    ImGui::Checkbox("Enable Specular Texture", &renderNewClassEnableSpecularTexture);
                                    if (renderNewClassEnableSpecularTexture)
                                    {
                                        ImGui::Combo("##SpecularTexture", &renderNewClassSpecularTextureCurrent,
                                            assetTexture.data(), assetTexture.size());
                                    }
                                }
                                else if (useShader == "Glass")
                                {
                                    ImGui::Text("N");
                                    ImGui::DragFloat("##N", &renderNewClassN, 0.01f, 1.0f, 2.2f);
                                }
                            }
                            ImGui::Separator();

                            bool canAddNewClass = checkSimpleStr(newClassName) &&
                                !classNameSet.count(newClassName);

                            if (!canAddNewClass) uiBeginDisable();

                            if (ImGui::Button("Add"))
                            {
                                addClassToProject(newClassName);

                                ClassItem newClass;
                                newClass.m_Name = std::string(newClassName);
                                newClass.m_Render = renderNewClass;
                                if (renderNewClass)
                                {
                                    if (Inside(renderNewClassModelCurrent, 0, assetModel.size() - 1))
                                    {
                                        newClass.m_Model = assetModel[renderNewClassModelCurrent];
                                    }

                                    std::string useShader = glManager.getSupportShaderAt(renderNewClassShaderCurrent);
                                    newClass.m_Shader = useShader;

                                    if (useShader == "Default")
                                    {
                                        newClass.m_DiffuseColor.x() = renderNewClassDiffuseColorf3[0];
                                        newClass.m_DiffuseColor.y() = renderNewClassDiffuseColorf3[1];
                                        newClass.m_DiffuseColor.z() = renderNewClassDiffuseColorf3[2];

                                        newClass.m_EnableDiffuseTexture = renderNewClassEnableDiffuseTexture;
                                        if (renderNewClassEnableDiffuseTexture &&
                                            Inside(renderNewClassDiffuseTextureCurrent, 0, assetTexture.size() - 1))
                                        {

                                            newClass.m_DiffuseTexture = assetTexture[renderNewClassDiffuseTextureCurrent];
                                        }

                                        newClass.m_EnableNormalTexture = renderNewClassEnableNormalTexture;
                                        if (renderNewClassEnableNormalTexture &&
                                            Inside(renderNewClassNormalTextureCurrent, 0, assetTexture.size() - 1))
                                        {
                                            newClass.m_NormalTexture = assetTexture[renderNewClassNormalTextureCurrent];
                                        }

                                        newClass.m_EnableSpecularTexture = renderNewClassEnableSpecularTexture;
                                        if (renderNewClassEnableSpecularTexture &&
                                            Inside(renderNewClassSpecularTextureCurrent, 0, assetTexture.size() - 1))
                                        {
                                            newClass.m_SpecularTexture = assetTexture[renderNewClassSpecularTextureCurrent];
                                        }
                                    }
                                    else if (useShader == "Glass")
                                    {
                                        newClass.m_N = renderNewClassN;
                                    }
                                }

                                classItems.push_back(newClass);

                                if (newClassAddActor)
                                {
                                    std::string defaultActorName = newClass.m_Name + "_Default";
                                    if (!actorNameSet.count(defaultActorName))
                                    {
                                        ActorItem newActor;
                                        newActor.m_Name = defaultActorName;
                                        newActor.m_ClassName = newClass.m_Name;
                                        newActor.m_Tags.insert("Actor_" + newClass.m_Name);

                                        newActor.m_Location = Eigen::Vector3d(0, 0, 0);
                                        newActor.m_Rotation = Eigen::Vector3d(0, 0, 0);
                                        newActor.m_Scale    = Eigen::Vector3d(1, 1, 1);

                                        actorItems.push_back(newActor);
                                    }
                                }

                                collectWorldSettings();
                                saveProject(worldSettings, actorItems, classItems, componentItems);

                                *newClassName    = 0;
                                newClassAddActor = false;
                                renderNewClass   = false;

                                renderNewClassModelCurrent = 0;

                                renderNewClassShaderCurrent = 0;

                                Fill(renderNewClassDiffuseColorf3, Color(1, 1, 1));

                                renderNewClassDiffuseTextureCurrent  = 0;
                                renderNewClassNormalTextureCurrent   = 0;
                                renderNewClassSpecularTextureCurrent = 0;

                                renderNewClassN = 1.4;

                                ImGui::CloseCurrentPopup();
                            }
                            if (!canAddNewClass) uiEndDisable();

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel"))
                            {
                                *newClassName    = 0;
                                newClassAddActor = false;
                                renderNewClass   = false;

                                renderNewClassModelCurrent = 0;

                                renderNewClassShaderCurrent = 0;

                                Fill(renderNewClassDiffuseColorf3, Color(1, 1, 1));

                                renderNewClassDiffuseTextureCurrent  = 0;
                                renderNewClassNormalTextureCurrent   = 0;
                                renderNewClassSpecularTextureCurrent = 0;

                                renderNewClassN = 1.4;

                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Component"))
                    {
                        if (ImGui::Button("New Component", toolboxButtonSize))
                        {
                            ImGui::OpenPopup("New Component");
                        }
                        if (ImGui::BeginPopupModal("New Component", nullptr, popWinFlag))
                        {
                            ImGui::Text("Guide for Create new Component");
                            ImGui::Separator();
                            ImGui::Text("Input the Component Name");
                            ImGui::InputText("", newComponentName, IM_ARRAYSIZE(newComponentName));

                            bool canAddComponent = checkSimpleStr(newComponentName) &&
                                !componentNameSet.count(std::string(newComponentName));

                            if (!canAddComponent) uiBeginDisable();
                            if (ImGui::Button("Add"))
                            {
                                ComponentItem newComponent;
                                newComponent.m_Name = std::string(newComponentName);

                                componentItems.push_back(newComponent);

                                addComponentToProject(newComponentName);

                                collectWorldSettings();
                                saveProject(worldSettings, actorItems, classItems, componentItems);

                                *newComponentName = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            if (!canAddComponent) uiEndDisable();

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel"))
                            {
                                *newComponentName = 0;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
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
                ImGui::PopItemWidth();

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
                    ImGui::InputTextMultiline("", allLogsBuffer, sizeof(allLogsBuffer), ImVec2(480, 200),
                        ImGuiInputTextFlags_ReadOnly);
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
        
        glViewport(0, 0, displayW, displayH);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
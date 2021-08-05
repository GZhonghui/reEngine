#include"VulkanManager.h"

bool VulkanManager::RenderUI()
{
    //Static
    static VulkanUIState uiState;

    //Layout
    const uint32_t ToolBoxWidth = 256;
    const uint32_t MenuBarHeight = 84;
    const uint32_t MainMenuHeight = 20;

    //Get States
    ImGuiIO IO = ImGui::GetIO();
    float displayWidth = (float)IO.DisplaySize.x;
    float displayHeight = (float)IO.DisplaySize.y;

    // Main Menu Bar
    {// Main Menu Bar Start
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About"))
                {
                    uiState.showAbout = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (uiState.showAbout)
        {
            if (ImGui::Begin("About reEngine", &uiState.showAbout, ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("reEngine");
                ImGui::Separator();
                ImGui::Text("reEngine is a OpenSource Game Engine");
                ImGui::Text("Coded by GZhonghui mail@gzher.com");

                ImGui::End();
            }
        }
    }// Main Menu Bar End

    // Menu Bar
    {// Menu Bar Start
        ImGui::SetNextWindowPos(ImVec2(ToolBoxWidth, MainMenuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(displayWidth - ToolBoxWidth, MenuBarHeight), ImGuiCond_Always);

        if (ImGui::Begin("reEngine MenuBar", nullptr, ImGuiWindowFlags_NoResize))
        {
            if (ImGui::BeginTabBar("Tool Tab"))
            {
                if (ImGui::BeginTabItem("Actors"))
                {
                    if (ImGui::Button("Add Actor"))
                    {

                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Delete Seleted"))
                    {

                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Load Model"))
                    {

                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Build"))
                {
                    if (ImGui::Button("Generate"))
                    {

                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }// Menu Bar End
    
    // Tool Box
    {// Tool Box Start
        ImGui::SetNextWindowPos(ImVec2(0, MainMenuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ToolBoxWidth, displayHeight - MainMenuHeight), ImGuiCond_Always);

        if (ImGui::Begin("reEngine ToolBox", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Now FPS: %d", m_FPS);
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Scene Actors"))
            {

            }

            if (ImGui::CollapsingHeader("Actor Properities"))
            {

            }

            if (ImGui::CollapsingHeader("Game Info"))
            {
                ImGui::Text("Camera Move Speed");
                ImGui::SliderFloat("", &Event::cameraMoveSpeed, 1.0f, 10.0f, "Speed = %.3f");
                ImGui::Separator();

                ImGui::Text("Camera Position");
                ImGui::Text("X = %.3f", Event::mainCamera.m_Position.x());
                ImGui::Text("Y = %.3f", Event::mainCamera.m_Position.y());
                ImGui::Text("Z = %.3f", Event::mainCamera.m_Position.z());
                ImGui::Separator();

                ImGui::Text("reEngine");
            }

            ImGui::End();
        }
    }// Tool Box End

    return true;
}